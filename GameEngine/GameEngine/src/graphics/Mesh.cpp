#include "stdafx.h"

#include "Mesh.h"

#include "utilities\WavefrontParser.h"
#include "utilities\MeshProcessor.h"

namespace engine {
namespace graphics {

  std::shared_ptr<Mesh> Mesh::Load(const String & _path)
  {
    std::shared_ptr<Mesh> mesh;
  
    try
    {
      file::WavefrontParser parser(_path);
      utilities::MeshProcessor processor;
      
      auto data = parser.getMesh();
  
      processor.CalculateTangents(
        data.indices,
        data.positions, data.uvs, data.normals,
        &data.tangents, &data.bitangents
      );
  
      mesh = std::make_shared<Mesh>(data);
    }
    catch (std::exception & _e)
    {
      debug::LogError(_e.what());
      return nullptr;
    }
  
    return mesh;
  }
  
  Mesh::Mesh() :
    m_draw(DrawType::TRIANGLES), m_elementCount(0), m_interleaved(true)
  { }
  
  Mesh::Mesh(const MeshData & _mesh, DrawType _draw) :
    m_draw(_draw), m_elementCount(0), m_interleaved(true)
  {
    if (_mesh.HasIndicies())
    {
      setIndices(_mesh.indices);
    }
  
    AddVertices(_mesh.positions.data(), _mesh.positions.size());
  
    if (_mesh.HasUVs())
    {
      AddUVs(_mesh.uvs.data(), _mesh.uvs.size());
    }
  
    if (_mesh.HasNormals())
    {
      AddNormals(_mesh.normals.data(), _mesh.normals.size());
    }
  
    if (_mesh.HasTangents())
    {
      AddAttribute(_mesh.tangents.data(), _mesh.tangents.size());
      AddAttribute(_mesh.bitangents.data(), _mesh.bitangents.size());
    }
  
    Apply();
  }
  
  Mesh::~Mesh()
  { }
  
  void Mesh::Render() const
  {
    if (!m_vao)
    {
      debug::LogError("Mesh " + getName() + " does not exist.");
      return;
    }
  
    m_vao->Bind();
  
  
    if (m_ibo)
    {
      m_ibo->Bind();
      m_ibo->Draw(m_draw);
    }
    else
    {
      GLCALL(glDrawArrays(DrawTypeToOpenGL(m_draw), 0, m_vertices.size()));
    }
  }
  
  void Mesh::setIndices(const Indices & _indices)
  {
    m_indices = _indices;
  }
  
  void Mesh::AddVertices(const glm::vec3 * _vertices, uint _count)
  {
    m_vertices.resize(_count);
    memcpy(&m_vertices[0], _vertices, sizeof(glm::vec3) * _count);
    AddAttribute<glm::vec3>(_vertices, _count);
  }
  
  void Mesh::AddUVs(const glm::vec2 * _uvs, uint _count)
  {
    AddAttribute<glm::vec2>(_uvs, _count);
  }
  
  void Mesh::AddNormals(const glm::vec3 * _normals, uint _count)
  {
    AddAttribute<glm::vec3>(_normals, _count);
  }
  
  void Mesh::Apply()
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
        layout.Add(attr.component, attr.componentCount, attr.typeSize, attr.normalized);
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
        layout.Add(attr.component, attr.componentCount, attr.size, attr.normalized);
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
  
    if (!m_indices.empty())
    {
      auto i = m_indices.get();
  
      m_ibo = std::make_unique<IndexBuffer>(&i.second[0], i.second.size(), i.first);
      m_ibo->Unbind();
    }
  
    UpdateBounds();
  }
  
  void Mesh::AddAttribute( 
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
  
  const Bounds & Mesh::getBounds() const
  {
    return m_bounds;
  }
  
  void Mesh::UpdateBounds()
  {
    if (m_vertices.empty()) 
    {
      m_bounds.min = m_bounds.max = glm::vec3(0.f);
      return;
    }
  
    m_bounds.min = m_bounds.max = m_vertices.front();
   
    for (size_t i = 1u; i < m_vertices.size(); ++i)
    {
      const glm::vec3 & pos = m_vertices[i];
      m_bounds.min.x = glm::min(m_bounds.min.x, pos.x);
      m_bounds.min.y = glm::min(m_bounds.min.y, pos.y);
      m_bounds.min.z = glm::min(m_bounds.min.z, pos.z);
  
      m_bounds.max.x = glm::max(m_bounds.min.x, pos.x);
      m_bounds.max.y = glm::max(m_bounds.min.y, pos.y);
      m_bounds.max.z = glm::max(m_bounds.min.z, pos.z);
    }
  }

} } // engine::graphics