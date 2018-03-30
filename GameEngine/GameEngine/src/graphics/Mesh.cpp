#include "stdafx.h"

#include "Mesh.h"

#include "debug\Debug.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<Mesh> Mesh::Load(const std::string & _path)
    {
      return nullptr;
    }

    Mesh::Mesh()
    { }

    Mesh::~Mesh()
    { }

    void Mesh::Render() const
    {
      //get currently active shader.
      //Render(Graphics::getActiveShader());
    }

    void Mesh::Render(Shader & _shader) const
    {
      if (!m_vbo || !m_indices)
      {
        debug::LogError("Mesh does not exist");
        return;
      }

      m_vbo->Bind();

      size_t attrCount = m_attributes.size();
      std::vector<ShaderAttribute> attrs(attrCount);

      for (size_t i = 0; i < attrCount; ++i)
      {
        const VertexAttribute & vertAttr = m_attributes[i];
        ShaderAttribute & attr = attrs[i];
        attr.location = -1;

        //if (!_shader.getAttribute(vertAttr.name, &attr))
        {
          if (!_shader.AddAttribute(vertAttr.name, vertAttr.type, vertAttr.count, false, &attr))
          {
            continue;
          }
        }

        if (attr.location < 0) { continue; }

        GLCALL(glEnableVertexAttribArray(attr.location));
        GLCALL(glVertexAttribPointer(attr.location, attr.count, attr.type, attr.normalized, 0, (const void *)vertAttr.offset));
      }


      m_indices->Bind();
      m_indices->Draw(m_draw);
      m_indices->Unbind();

      for (auto & attr : attrs)
      {
        if (attr.location >= 0)
        {
          GLCALL(glDisableVertexAttribArray(attr.location));
        }
      }

      m_vbo->Unbind();
    }

    void Mesh::setIndices(const uint8 * _indices, uint _count)
    {
      m_indexData.resize(sizeof(uint8) * _count);
      memcpy(&m_indexData[0], _indices, sizeof(uint8) * _count);
      m_indexType = GL_UNSIGNED_BYTE;
    }

    void Mesh::setIndices(const uint16 * _indices, uint _count)
    {
      m_indexData.resize(sizeof(uint16) * _count);
      memcpy(&m_indexData[0], _indices, sizeof(uint16) * _count);
      m_indexType = GL_UNSIGNED_SHORT;
    }

    void Mesh::setIndices(const uint32 * _indices, uint _count)
    {
      m_indexData.resize(sizeof(uint32) * _count);
      memcpy(&m_indexData[0], _indices, sizeof(uint32) * _count);
      m_indexType = GL_UNSIGNED_INT;
    }

    void Mesh::setVertices(const glm::vec3 * _vertices, uint _count)
    {
      setAttribute<glm::vec3>(Shader::ATTR_POSITION_NAME, _vertices, _count);
      m_vertices.resize(_count);
      memcpy(&m_vertices[0], _vertices, sizeof(glm::vec3) * _count);
    }

    void Mesh::setUVs(const glm::vec2 * _uvs, uint _count)
    {
      setAttribute<glm::vec2>(Shader::ATTR_UV_NAME, _uvs, _count);
    }

    void Mesh::setNormals(const glm::vec3 * _normals, uint _count)
    {
      setAttribute<glm::vec3>(Shader::ATTR_NORMAL_NAME, _normals, _count);
    }

    void Mesh::Apply()
    {
      m_vbo.reset(new VertexBuffer());
      m_vbo->setData(&m_vboData[0], m_vboData.size());
      m_vbo->Unbind();

      try
      {
        m_indices.reset(new IndexBuffer(&m_indexData[0], m_indexData.size(), m_indexType));
        m_indices->Unbind();
      }
      catch (std::invalid_argument & _e)
      {
        //TODO: name.
        debug::LogError("Mesh Error: " + std::string(_e.what()));

        m_vbo.release();
        m_indices.release();
      }
    }

    void Mesh::AddAttribute(const std::string & _name, const void * _data, uint _size, GLenum _type, uint _count)
    {
      if (getAttribute(_name))
      {
        //TODO: name.
        debug::LogWarning("Mesh: attribute " + _name + " already exists, setting attribute data.");
        setAttribute(_name, _data, _size, _type, _count);
        return;
      }

      uint start = m_vboData.size();
      m_vboData.resize(start + _size);

      memcpy(&m_vboData[start], _data, _size);

      VertexAttribute attr = {
        _name, 
        start,
        _size,
        _type,
        _count
      };

      m_attributes.push_back(attr);
      m_nameToAttribute[_name] = m_attributes.size() - 1;
    }

    void Mesh::setAttribute(const std::string & _name, const void * _data, uint _size, GLenum _type, uint _count)
    {
      VertexAttribute attr;
      if (!getAttribute(_name, &attr))
      {
        //TODO: name.
        //debug::LogWarning("Mesh: attribute " + _name + " does not exist, creating attribute.");
        AddAttribute(_name, _data, _size, _type, _count);
        return;
      }

      if (attr.size != _size)
      {
        //TODO: name.
        throw std::out_of_range(
          "Mesh Error: set vertex attribute " + _name + " with different data size. expected: " +
          std::to_string(attr.size) + ", actual: " + std::to_string(_size)
        );
      }

      memcpy(&m_vboData[attr.offset], _data, _size);
    }

    bool Mesh::getAttribute(const std::string & _name, VertexAttribute * _outAttribute) const
    {
      auto attribute = m_nameToAttribute.find(_name);
      if (attribute != m_nameToAttribute.end()) { return false; }
      
      if (_outAttribute != nullptr)
      {
        *_outAttribute = m_attributes[attribute->second];
      }
      return true;
    }
  }
}