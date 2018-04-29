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
        mesh->AddVertices(&vertices[0], vertices.size());
        
        if (parser.HasUVs())
        {
          auto uvs = parser.getUVs();
          mesh->AddUVs(&uvs[0], uvs.size());
        }

        if (parser.HasNormals())
        {
          auto normals = parser.getNormals();
          mesh->AddNormals(&normals[0], normals.size());
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

          mesh->AddAttribute(Shader::ATTR_TANGENT_NAME, &tangents[0], tangents.size());
          mesh->AddAttribute(Shader::ATTR_BITANGENT_NAME, &bitangents[0], bitangents.size());
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
      m_draw(DrawType::TRIANGLES), m_interleaved(true)
    { }

    Mesh::~Mesh()
    { }

    void Mesh::Render() const
    {
      if (!m_vao || !m_indices)
      {
        debug::LogError("Mesh " + getName() + " does not exist.");
        return;
      }

      m_vao->Bind();

      m_indices->Bind();
      m_indices->Draw(m_draw);
      m_indices->Unbind();

      m_vao->Unbind();
    }

    void Mesh::setIndices(const uint8 * _indices, uint _count)
    {
      m_indexData.resize(sizeof(uint8) * _count);
      memcpy(&m_indexData[0], _indices, sizeof(uint8) * _count);
      m_indexType = IndexType::UNSIGNED_BYTE;
    }

    void Mesh::setIndices(const uint16 * _indices, uint _count)
    {
      m_indexData.resize(sizeof(uint16) * _count);
      memcpy(&m_indexData[0], _indices, sizeof(uint16) * _count);
      m_indexType = IndexType::UNSIGNED_SHORT;
    }

    void Mesh::setIndices(const uint32 * _indices, uint _count)
    {
      m_indexData.resize(sizeof(uint32) * _count);
      memcpy(&m_indexData[0], _indices, sizeof(uint32) * _count);
      m_indexType = IndexType::UNSIGNED_INT;
    }

    void Mesh::AddVertices(const glm::vec3 * _vertices, uint _count)
    {
      m_vertices.resize(_count);
      memcpy(&m_vertices[0], _vertices, sizeof(glm::vec3) * _count);
      AddAttribute<glm::vec3>(Shader::ATTR_POSITION_NAME, _vertices, _count);
    }

    void Mesh::AddUVs(const glm::vec2 * _uvs, uint _count)
    {
      AddAttribute<glm::vec2>(Shader::ATTR_UV_NAME, _uvs, _count);
    }

    void Mesh::AddNormals(const glm::vec3 * _normals, uint _count)
    {
      AddAttribute<glm::vec3>(Shader::ATTR_NORMAL_NAME, _normals, _count);
    }

    void Mesh::Apply()
    {
      try
      {
        if (m_data.empty())
        {
          throw std::range_error("No vertex attribute data");
        }

        std::vector<byte> vboData;
        BufferLayout layout;

        if (m_interleaved)
        {
          for (size_t i = 0; i < m_attributes.size(); ++i)
          {
            const auto & attr = m_attributes[i];
            layout.Add(attr.name, attr.component, attr.componentCount, attr.typeSize, attr.normalized);
          }

          vboData.resize(m_elementCount * layout.getSize());

          for (size_t i = 0; i < m_attributes.size(); ++i)
          {
            const auto & attr = m_attributes[i];
            const auto & data = m_data[i];
            
            const uint & offset = layout.at(i).offset;

            for (size_t j = 0; j < m_elementCount; ++j)
            {
              uint stride = layout.getSize() * j;

              size_t index = j * attr.typeSize;
              memcpy(&vboData[stride + offset], &data[index], attr.typeSize);
            }
          }
        }
        else
        {
          for (size_t i = 0; i < m_attributes.size(); ++i)
          {
            const auto & attr = m_attributes[i];
            layout.Add(attr.name, attr.component, attr.componentCount, attr.size, attr.normalized);
          }

          vboData.resize(layout.getSize());

          for (size_t i = 0; i < m_attributes.size(); ++i)
          {
            const auto & attr = m_attributes[i];
            const auto & data = m_data[i];

            uint offset = layout.at(i).offset;

            memcpy(&vboData[offset], &data[0], data.size());
          }
        }

        m_vao = std::make_unique<VertexArray>();

        auto vbo = std::make_unique<VertexBuffer>();
        vbo->setData(&vboData[0], vboData.size());
        vbo->setLayout(layout);

        m_vao->AddBuffer(std::move(vbo), m_interleaved);

        m_vao->Unbind();

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

        m_vao.release();
        m_indices.release();
      }
    }

    void Mesh::AddAttribute(
      const std::string & _name, 
      const void * _data, uint _size, 
      uint _count, 
      Type _type,
      ComponentType _component, uint _componentCount,
      bool _normalized
    )
    {
      if (m_elementCount == 0)
      {
        m_elementCount = _count;
      }
      else if (_count != m_elementCount)
      {
        throw std::range_error("Invalid amount of elements");
      }

      VertexAttribute attr;
      attr.name = _name;
      attr.size = _size;
      attr.type = _type;
      attr.typeSize = _size / _count;
      attr.component = _component;
      attr.componentCount = _componentCount;
      attr.normalized = _normalized;

      m_attributes.push_back(attr);

      std::vector<byte> data(_size);
      memcpy(&data[0], _data, _size);
      m_data.push_back(std::move(data));
    }

    void Mesh::ClearAttributes()
    {
      m_attributes.clear();
      m_data.clear();
    }

    void Mesh::setDraw(DrawType _draw)
    {
      m_draw = _draw;
    }
  }
}