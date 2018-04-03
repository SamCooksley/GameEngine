#include "stdafx.h"

#include "Material.h"

#include "Graphics.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<Material> Material::Create(std::shared_ptr<Shader> _shader)
    {
      struct enable_mat : public Material { };

      auto material = std::make_shared<enable_mat>();
      material->setShader(_shader);
      return material;
    }

    std::shared_ptr<Material> Material::Create(std::shared_ptr<Material> _material)
    {
      auto mat = Create(_material->getShader());
      mat->m_uniformData = _material->m_uniformData;
      return mat;
    }

    Material::Material()
    { }

    Material::~Material()
    {
      RemoveFromShader();
    }

    void Material::Bind() const
    {
      m_shader->Bind();

      for (auto & uniform : m_shader->m_uniforms)
      {
        m_shader->setUniform(uniform.location, uniform.type, &m_uniformData[uniform.offset]);
      }

      for (size_t i = 0; i < m_textures.size(); ++i)
      {
        m_shader->setUniform(m_shader->m_textures[i].location, static_cast<int>(m_shader->m_textures[i].textureUnit));
        if (m_textures[i])
        {
          m_textures[i]->Bind(i);
        }
      }
    }

    void Material::Unbind() const
    {
      for (size_t i = 0; i < m_textures.size(); ++i)
      {
        if (m_textures[i])
        {
          m_textures[i]->Unbind(i);
        }
      }

      m_shader->Unbind();
    }

    void Material::setUniform(const std::string & _name, const void * _data, uint _size, GLenum _type)
    {
      ShaderUniform uniform;
      if (!m_shader->AddUniform(_name, _type, _size, &uniform))
      {
        debug::LogError("Material Error: failed to find uniform " + _name + " in material " + getName());
        return;
      }

      if (uniform.type != _type)
      {
        throw std::invalid_argument(
          "Material Error: " + _name + " uniform type does not match in material " + getName() +
          ". expected: " + std::to_string(uniform.type) + ", actual: " + std::to_string(_type)
        );
      }

      memcpy(&m_uniformData[uniform.offset], _data, _size);
    }

    bool Material::getUniform(const std::string & _name, void * _outValue, uint _size, GLenum _type) const
    {
      ShaderUniform uniform;
      if (!m_shader->getUniform(_name, &uniform))
      {
        return false;
      }

      if (uniform.type != _type)
      {
        return false;
      }

      if (_outValue != nullptr)
      {
        memcpy(_outValue, &m_uniformData[uniform.offset], _size);
      }

      return true;
    }

    void Material::setTexture(const std::string & _name, std::shared_ptr<Texture> _texture)
    {
      ShaderTexture texture;
      if (!m_shader->AddTexture(_name, &texture))
      {
        debug::LogError("Material Error: failed to find texture " + _name + " in material " + getName());
        return;
      }

      if (texture.textureUnit >= m_textures.size())
      {
        throw std::out_of_range("Material Error: texture unit out of range in " +
          getName() + ". max: " + std::to_string(m_textures.size()) + ", index: " +
          std::to_string(texture.textureUnit)
        );
      }

      m_textures.at(texture.textureUnit) = std::move(_texture);
    }

    bool Material::getTexture(const std::string & _name, std::shared_ptr<Texture> * _outTexture) const
    {
      ShaderTexture texture;
      if (!m_shader->getTexture(_name, &texture))
      {
        return false;
      }

      if (_outTexture != nullptr)
      {
        if (texture.textureUnit >= m_textures.size())
        {
          throw std::out_of_range("Material Error: texture unit out of range in " +
            getName() + ". max: " + std::to_string(m_textures.size()) + ", index: " +
            std::to_string(texture.textureUnit)
          );
        }

        *_outTexture = m_textures.at(texture.textureUnit);
      }

      return true;
    }

    std::shared_ptr<Shader> Material::getShader()
    {
      return m_shader;
    }

    void Material::setShader(std::shared_ptr<Shader> _shader)
    {
      RemoveFromShader();

      m_shader = std::move(_shader);

      if (m_shader)
      {
        m_shader->AddMaterial(this);
      }
      else
      {
        debug::LogError("Material Error: material " + getName() + " does not have a shader.");
        m_shader = Graphics::getErrorShader();
        if (m_shader)
        {
          m_shader->AddMaterial(this);
        }
      }

      UpdateSizes();
      Reset();
    }

    void Material::RemoveFromShader()
    {
      if (m_shader)
      {
        m_shader->RemoveMaterial(this);
      }
      m_shader = nullptr;
    }

    void Material::UpdateSizes()
    {
      if (m_shader)
      {
        m_uniformData.resize(m_shader->m_uniformSize);
        m_textures.resize(m_shader->m_textures.size());
      }
      else
      {
        m_uniformData.clear();
        m_textures.clear();
      }
    }

    void Material::Reset()
    {
      if (!m_uniformData.empty())
      {
        memset(&m_uniformData[0], 0, m_uniformData.size());
      }

      for (auto & texture : m_textures)
      {
        texture.reset();
      }
    }
  }
}