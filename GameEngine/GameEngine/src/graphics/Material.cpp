#include "stdafx.h"

#include "Material.h"

#include "Graphics.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<Material> Material::CreateInternal(std::shared_ptr<Shader> _shader)
    {
      struct enable_mat : public Material { };

      auto material = std::make_shared<enable_mat>();

      if (!_shader)
      {
        debug::LogError("Material does not have a shader.");
        material->m_shader = Graphics::getContext().errorShader;
      }
      else
      {
        material->m_shader = std::move(_shader);
      }

      return material;
    }

    std::shared_ptr<Material> Material::Create(std::shared_ptr<Shader> _shader)
    {
      auto mat = CreateInternal(_shader);
      mat->m_shader->RetreiveUniformData(mat->m_uniformData);
   
      for (auto & sampler : mat->m_shader->m_samplers)
      {
        mat->setUniform<int>(sampler.name, -1);
      }

      return mat;
    }

    std::shared_ptr<Material> Material::Create(std::shared_ptr<Material> _material)
    {
      auto mat = CreateInternal(_material->getShader());
      mat->m_uniformData = _material->m_uniformData;
      mat->m_textures = _material->m_textures;
      return mat;
    }

    Material::Material()
    { }

    Material::~Material()
    { }

    void Material::Bind() const
    {
      m_shader->Bind();

      for (auto & uniform : m_shader->m_uniforms)
      {
        m_shader->setUniform(uniform.location, uniform.type, &m_uniformData[uniform.offset]);
      }

      for (auto & unit : m_textures)
      {
        unit.texture->Bind(unit.unit);
      }
    }

    void Material::Unbind() const
    {
      for (auto & unit : m_textures)
      {
        unit.texture->Unbind(unit.unit);
      }

      m_shader->Unbind();
    }

    Material::Error Material::setTexture(const std::string & _name, std::shared_ptr<Texture2D> _texture, bool _new)
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return Error::INVALID_UNIFORM;
      }

      return setTexture(sampler, TextureType::TEXTURE_2D, _texture, _new);
    }

    Material::Error Material::setTexture(const std::string & _name, std::shared_ptr<TextureCube> _textureCube, bool _new)
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return Error::INVALID_UNIFORM;
      }

      return setTexture(sampler, TextureType::TEXTURE_CUBE, _textureCube, _new);
    }

    Material::Error Material::setTexture(const ShaderSampler & _sampler, TextureType _type, std::shared_ptr<Texture> _texture, bool _new)
    {
      if (_sampler.type != _type)
      {
        return Error::INVALID_TYPE;
      }

      int unit;
      Error error = getTextureUnit(_sampler, &unit);
      if (error != Error::OK)
      {
        return error;
      }

      auto texture = m_textures.find(unit);
      if (!_new && texture != m_textures.mend())
      {
        assert(m_textures[texture->second].texture->type == _type);

        m_textures[texture->second].texture = std::move(_texture);
      }
      else
      {
        TextureUnit texture;
        texture.count = 0;
        texture.unit = FindFreeTextureUnit();
        texture.texture = std::move(_texture);

        m_textures.add(texture.unit, texture);

        error = setTextureUnit(_sampler, texture.unit);
        if (error != Error::OK)
        {
          return error;
        }
      }

      return Error::OK;
    }

    Material::Error Material::getTexture(const std::string & _name, std::shared_ptr<Texture2D> * _outTexture) const
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return Error::INVALID_UNIFORM;
      }

      if (sampler.type != TextureType::TEXTURE_2D)
      {
        return Error::INVALID_TYPE;
      }

      int unit;
      Error error = getTextureUnit(_name, &unit);
      if (error != Error::OK)
      {
        return error;
      }

      if (unit < 0)
      {
        if (_outTexture != nullptr)
        {
          *_outTexture = nullptr;
        }
        return Error::OK;
      }

      auto texture = m_textures.find(unit);
      assert(texture != m_textures.mend());

      if (_outTexture != nullptr)
      {
        *_outTexture = std::dynamic_pointer_cast<Texture2D>(m_textures[texture->second].texture);
      }

      return Error::OK;
    }

    Material::Error Material::getTexture(const std::string & _name, std::shared_ptr<TextureCube> * _outTextureCube) const
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return Error::INVALID_UNIFORM;
      }

      if (sampler.type != TextureType::TEXTURE_CUBE)
      {
        return Error::INVALID_TYPE;
      }

      int unit;
      Error error = getTextureUnit(_name, &unit);
      if (error != Error::OK)
      {
        return error;
      }

      if (unit < 0)
      {
        if (_outTextureCube != nullptr)
        {
          *_outTextureCube = nullptr;
        }
        return Error::OK;
      }

      auto texture = m_textures.find(unit);
      assert(texture != m_textures.mend());

      if (_outTextureCube != nullptr)
      {
        *_outTextureCube = std::static_pointer_cast<TextureCube>(m_textures[texture->second].texture);
      }

      return Error::OK;
    }

    Material::Error Material::setTextureUnit(const std::string & _name, int _unit)
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return Error::INVALID_UNIFORM;
      }

      return setTextureUnit(sampler, _unit);
    }

    Material::Error Material::setTextureUnit(const ShaderSampler & _sampler, int _unit)
    {
      auto textureIter = m_textures.find(_unit);
      if (textureIter == m_textures.mend())
      {
        return Error::INVALID_TEXTURE_UNIT;
      }

      TextureUnit & texture = m_textures[textureIter->second];

      if (texture.texture->type != _sampler.type)
      {
        return Error::INVALID_TYPE;
      }

      int oldUnit;
      Error error = getTextureUnit(_sampler, &oldUnit);
      if (error != Error::OK)
      {
        return error;
      }

      if (oldUnit == _unit)
      {
        return Error::OK;
      }

      //remove the reference from the old unit.
      auto oldTexture = m_textures.find(oldUnit);
      if (oldTexture != m_textures.mend())
      {
        if (--m_textures[oldTexture->second].count <= 0)
        {
          //if there are no more references, delete the unit.
          m_textures.remove(oldTexture);
        }
      }

      error = setUniform<int>(m_shader->m_uniforms[_sampler.uniformIndex], _unit);
      if (error != Error::OK)
      {
        return error;
      }
      ++texture.count;

      return Error::OK;
    }

    Material::Error Material::getTextureUnit(const std::string & _name, int * _unit) const
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return Error::INVALID_UNIFORM;
      }

      return getTextureUnit(sampler, _unit);
    }

    Material::Error Material::getTextureUnit(const ShaderSampler & _sampler, int * _unit) const
    {
      return getUniform<int>(m_shader->m_uniforms[_sampler.uniformIndex], _unit);
    }

    int Material::FindFreeTextureUnit() const
    {
      int unit = 0;
      while (m_textures.exists(unit))
      {
        ++unit;
      }
      return unit;
    }

    const std::shared_ptr<Shader> & Material::getShader()
    {
      return m_shader;
    }
  }
}