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

    void Material::setUniform(const std::string & _name, const void * _data, uint _size, GLenum _type)
    {
      ShaderUniform uniform;
      if (!m_shader->getUniform(_name, &uniform))
      {
        throw std::invalid_argument("Uniform does not exist.");
      }

      if (uniform.type != _type)
      {
        throw std::invalid_argument("Uniform not of type.");
      }

      assert(uniform.offset + _size <= m_uniformData.size());

      memcpy(&m_uniformData[uniform.offset], _data, _size);
    }

    bool Material::getUniform(const std::string & _name, void * _outValue, uint _size, GLenum _type) const
    {
      ShaderUniform uniform;
      if (!m_shader->getUniform(_name, &uniform))
      {
        return false;
      }

      if (_type != uniform.type)
      {
        return false;
      }

      assert(uniform.offset + _size <= m_uniformData.size());

      if (_outValue != nullptr)
      {
        memcpy(_outValue, &m_uniformData[uniform.offset], _size);
      }

      return true;
    }

    void Material::setTexture(const std::string & _name, std::shared_ptr<Texture2D> _texture)
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        throw std::invalid_argument("Sampler does not exist.");
      }

      if (sampler.type != GL_SAMPLER_2D)
      {
        throw std::invalid_argument("Uniform not of type.");
      }

      setTexture(_name, GL_SAMPLER_2D, _texture);
    }

    void Material::setTexture(const std::string & _name, std::shared_ptr<TextureCube> _textureCube)
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        throw std::invalid_argument("Sampler does not exist.");
      }

      if (sampler.type != GL_SAMPLER_CUBE)
      {
        throw std::invalid_argument("Uniform not of type.");
      }

      setTexture(_name, GL_SAMPLER_CUBE, _textureCube);
    }

    void Material::setTextureUnit(const std::string & _name, int _unit)
    {
      auto textureIter = m_textures.find(_unit);
      if (textureIter == m_textures.mend())
      {
        throw std::invalid_argument("Texture unit not in use.");
      }

      TextureUnit & texture = m_textures[textureIter->second];

      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        throw std::invalid_argument("Sampler does not exist.");
      }

      if (texture.type != sampler.type)
      {
        throw std::invalid_argument("Texture not of same type as sampler.");
      }

      int oldUnit = getTextureUnit(_name);
      auto oldTexture = m_textures.find(oldUnit);
      if (oldTexture != m_textures.mend())
      {
        if (--m_textures[oldTexture->second].count <= 0)
        {
          m_textures.remove(oldTexture);
        }        
      }

      setUniform<int>(_name, _unit);
      ++texture.count;
    }

    int Material::getTextureUnit(const std::string & _name) const
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        throw std::invalid_argument("Sampler does not exist.");
      }

      int unit;
      getUniform<int>(_name, &unit);
      return unit;
    }

    bool Material::getTexture(const std::string & _name, std::shared_ptr<Texture2D> * _outTexture) const
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return false;
      }

      if (sampler.type != GL_SAMPLER_2D)
      {
        return false;
      }

      int unit = getTextureUnit(_name);

      auto texture = m_textures.find(unit);
      assert(texture != m_textures.mend());

      if (_outTexture != nullptr)
      {
        *_outTexture = std::dynamic_pointer_cast<Texture2D>(m_textures[texture->second].texture);
      }

      return true;
    }

    bool Material::getTexture(const std::string & _name, std::shared_ptr<TextureCube> * _outTextureCube) const
    {
      ShaderSampler sampler;
      if (!m_shader->getSampler(_name, &sampler))
      {
        return false;
      }

      if (sampler.type != GL_SAMPLER_CUBE)
      {
        return false;
      }

      int unit = getTextureUnit(_name);

      auto texture = m_textures.find(unit);
      assert(texture != m_textures.mend());

      if (_outTextureCube != nullptr)
      {
        *_outTextureCube = std::static_pointer_cast<TextureCube>(m_textures[texture->second].texture);
      }

      return true;
    }

    void Material::setTexture(const std::string & _name, GLenum _type, std::shared_ptr<Texture> _texture)
    {
      int unit = getTextureUnit(_name);

      auto texture = m_textures.find(unit);
      if (texture != m_textures.mend())
      {
        assert(m_textures[texture->second].type == _type);

        m_textures[texture->second].texture = std::move(_texture);
      }
      else
      {
        TextureUnit texture;
        texture.type = _type;
        texture.count = 1;
        texture.unit = FindFreeTextureUnit();
        texture.texture = std::move(_texture);

        m_textures.add(texture.unit, texture);

        setTextureUnit(_name, texture.unit);
      }
    }

    bool Material::getTextureInfo(int _unit, TextureUnit * _outTexture) const
    {
      auto unit = m_textures.find(_unit);
      if (unit == m_textures.mend()) { return false; }

      if (_outTexture != nullptr)
      {
        *_outTexture = m_textures[unit->second];
      }

      return true;
    }

    int Material::FindFreeTextureUnit() const
    {
      int result = 0;
      while (m_textures.find(result) != m_textures.mend())
      {
        ++result;
      }
      return result;
    }

    std::shared_ptr<Shader> Material::getShader()
    {
      return m_shader;
    }
  }
}