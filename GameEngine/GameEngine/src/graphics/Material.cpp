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
    }

    void Material::Unbind() const
    {
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
      }
      else
      {
        m_uniformData.clear();
      }
    }

    void Material::Reset()
    {
      if (!m_uniformData.empty())
      {
        memset(&m_uniformData[0], 0, m_uniformData.size());
      }
    }
  }
}