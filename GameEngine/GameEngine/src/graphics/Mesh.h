#ifndef _ENGINE_GRAPHICS_MESH_H_
#define _ENGINE_GRAPHICS_MESH_H_

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include "Asset.h"

#include "MeshData.h"

#include "Bounds.h"

namespace engine {
namespace graphics {

  struct VertexAttribute
  {
   public:
    uint size;
    Type type;
    uint typeSize;
    ComponentType component;
    uint componentCount;
    bool normalized;
  };
  
  class Mesh : public NonCopyable, public NamedObject, public Asset
  {
   public:
    static std::shared_ptr<Mesh> Load(const String & _path);
  
    Mesh();
    Mesh(const MeshData & _data, DrawType _draw = DrawType::TRIANGLES);
    ~Mesh();
  
    void Render() const;
  
    void setIndices(const Indices & _indices);
  
    void AddVertices(const glm::vec3 * _vertices, uint _count);
    void AddUVs(const glm::vec2 * _uvs, uint _count);
    void AddNormals(const glm::vec3 * _normals, uint _count);
  
    template <typename T>
    void AddAttribute(
      const T * _data, uint _count, 
      bool _normalized = false
    );
  
    void Apply();
  
    void setDraw(DrawType _draw);
  
    const Bounds & getBounds() const;
  
   private:
    void AddAttribute(
      const void * _data, uint _size, 
      uint _count,
      Type _type, 
      ComponentType _component,
      uint _componentCount,
      bool _normalized = false
    );
  
    void ClearAttributes();
  
    void UpdateBounds();
  
   private:
    std::unique_ptr<IndexBuffer> m_ibo;
    std::unique_ptr<VertexArray> m_vao;
  
    DrawType m_draw;
    
    std::vector<glm::vec3> m_vertices;
  
    std::vector<VertexAttribute> m_attributes;
    std::vector<std::vector<byte>> m_data;
    uint m_elementCount;
    bool m_interleaved;
  
    Indices m_indices;
  
    Bounds m_bounds;
  };
  
} } // engine::graphics

#include "Mesh.inl"

#endif //_ENGINE_GRAPHICS_MESH_H_