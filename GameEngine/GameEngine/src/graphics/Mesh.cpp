#include "stdafx.h"

#include "Mesh.h"

#include "utilities\WavefrontParser.h"
#include "utilities\MeshProcessor.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<Mesh> Mesh::Create()
    {
      class enable_mesh : public Mesh { };
      return std::make_shared<enable_mesh>();
    }

    std::shared_ptr<Mesh> Mesh::Load(const std::string & _path)
    {
      std::shared_ptr<Mesh> mesh;

      try
      {
        file::WavefrontParser parser(_path);
        utilities::MeshProcessor processor;
        mesh = Create();
        mesh->setName(parser.getName());

        auto vertices = parser.getVertices();
        mesh->setVertices(&vertices[0], vertices.size());
        
        if (parser.HasUVs())
        {
          auto uvs = parser.getUVs();
          mesh->setUVs(&uvs[0], uvs.size());
        }

        if (parser.HasNormals())
        {
          auto normals = parser.getNormals();
          mesh->setNormals(&normals[0], normals.size());
        }

        if (parser.HasUVs() && parser.HasNormals())
        {
          std::vector<glm::vec3> tangents, bitangents;
          processor.CalculateTangents(
            parser.getIndices(),
            parser.getVertices(),
            parser.getUVs(),
            parser.getNormals(),
            &tangents,
            &bitangents
          );

          mesh->setAttribute(Shader::ATTR_TANGENT_NAME, &tangents[0], tangents.size());
          mesh->setAttribute(Shader::ATTR_BITANGENT_NAME, &bitangents[0], bitangents.size());
        }

        if (parser.CanUseIndex8())
        {
          std::vector<uint8> indices;
          parser.getIndices(indices);

          mesh->setIndices(&indices[0], indices.size());
        }
        else if (parser.CanUseIndex16())
        {
          std::vector<uint16> indices;
          parser.getIndices(indices);

          mesh->setIndices(&indices[0], indices.size());
        }
        else
        {
          std::vector<uint32> indices;
          parser.getIndices(indices);

          mesh->setIndices(&indices[0], indices.size());
        }  

        mesh->Apply();
      }
      catch (std::exception & _e)
      {
        debug::LogError(_e.what());
        return nullptr;
      }

      return mesh;
    }

    Mesh::Mesh() :
      m_draw(DrawType::Triangles)
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
        debug::LogError("Mesh " + getName() + " does not exist.");
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

        if (!_shader.getAttribute(vertAttr.name, &attr))
        {
          continue;
        }

        if (attr.location < 0) { continue; }

        if (attr.type != vertAttr.type)
        {
          debug::LogError(
            "Mesh Error: " + attr.name + " not of type " + std::to_string(vertAttr.type)
            + ". Expected " + std::to_string(attr.type)
          );
          continue;
        }

        GLCALL(glEnableVertexAttribArray(attr.location));
        GLCALL(glVertexAttribPointer(
          attr.location, 
          vertAttr.count,
          vertAttr.component,
          vertAttr.normalized,
          0, 
          reinterpret_cast<const void *>(vertAttr.offset)
        ));
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
      ClearAttributes();
      m_vertices.resize(_count);
      memcpy(&m_vertices[0], _vertices, sizeof(glm::vec3) * _count);
      setAttribute<glm::vec3>(Shader::ATTR_POSITION_NAME, _vertices, _count);
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
      try
      {
        if (m_vboData.empty())
        {
          throw std::range_error("Mesh Error: no vertex buffer data given in mesh " + getName());
        }

        m_vbo.reset(new VertexBuffer());
        m_vbo->setData(&m_vboData[0], m_vboData.size());
        m_vbo->Unbind();

        if (m_indexData.empty())
        {
          throw std::range_error("Mesh Error: no index data given in mesh " + getName());
        }
        m_indices.reset(new IndexBuffer(&m_indexData[0], m_indexData.size(), m_indexType));
        m_indices->Unbind();
      }
      catch (std::exception & _e)
      {
        debug::LogError(std::string(_e.what()));

        m_vbo.release();
        m_indices.release();
      }
    }

    void Mesh::AddAttribute(
      const std::string & _name, 
      const void * _data, uint _size, 
      uint _count, 
      GLenum _type,
      bool _normalized
    )
    {
      if (m_vertices.empty())
      {
        throw std::range_error(
          "Mesh Error: attempted to set attribute " + _name + " before vertex data in mesh " +
          getName() + ". Vertex attribute must be set first"
        );
      }

      if (_count != m_vertices.size())
      {
        throw std::range_error(
          "Mesh Error: attribute " + _name + " contains different number of elements to vertices in mesh " + 
          getName() + ". expected: " + std::to_string(m_vertices.size()) + ", actual: " + std::to_string(_count)
        );
      }

      if (getAttribute(_name))
      {
        debug::LogWarning("Mesh: attribute " + _name + " already exists in mesh " + getName() + ", setting attribute data.");
        setAttribute(_name, _data, _size, _count, _type, _normalized);
        return;
      }

      uint start = m_vboData.size();
      m_vboData.resize(start + _size);

      memcpy(&m_vboData[start], _data, _size);

      VertexAttribute attr;
      attr.name = _name;
      attr.offset = start;
      attr.size = _size;
      attr.type = _type;
      attr.component = GetComponentType(_type);
      attr.count = GetComponentCount(_type);
      attr.normalized = _normalized;

      m_attributes.add(_name, attr);
    }

    void Mesh::setAttribute(
      const std::string & _name, 
      const void * _data, uint _size, 
      uint _count,
      GLenum _type, 
      bool _normalized
    )
    {
      VertexAttribute attr;
      if (!getAttribute(_name, &attr))
      {
        AddAttribute(_name, _data, _size, _count, _type, _normalized);
        return;
      }

      RemoveAttribute(_name);
      AddAttribute(_name, _data, _size, _count, _type, _normalized);
    }

    bool Mesh::getAttribute(const std::string & _name, VertexAttribute * _outAttribute) const
    {
      auto attribute = m_attributes.find(_name);
      if (attribute == m_attributes.mend()) { return false; }
      
      if (_outAttribute != nullptr)
      {
        *_outAttribute = m_attributes[attribute->second];
      }
      return true;
    }

    void Mesh::RemoveAttribute(const std::string & _name)
    {
      VertexAttribute attribute;
      if (!getAttribute(_name, &attribute))
      {
        return;
      }

      uint start = attribute.offset;
      uint end = attribute.offset + attribute.size;

      m_vboData.erase(std::begin(m_vboData) + start, std::begin(m_vboData) + end);

      
      size_t index = m_attributes.find(_name)->second;
      m_attributes.remove(_name);

      for (size_t i = index; i < m_attributes.size(); ++i)
      {
        VertexAttribute & attr = m_attributes[i];
        attr.offset = start;

        start += attr.size;
      }
    }

    void Mesh::ClearAttributes()
    {
      m_attributes.clear();
      m_vboData.clear();
    }
  }
}