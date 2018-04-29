#ifndef _ENGINE_GRAPHICS_MESH_H_
#define _ENGINE_GRAPHICS_MESH_H_

#include "core\Object.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include "Shader.h"

namespace engine
{
  namespace graphics
  {
    struct VertexAttribute
    {
    public:
      std::string name;
      uint size;
      Type type;
      uint typeSize;
      ComponentType component;
      uint componentCount;
      bool normalized;
    };

    class Mesh : public Object, public NonCopyable
    {
    public:
      static std::shared_ptr<Mesh> Create();
      static std::shared_ptr<Mesh> Load(const std::string & _path);

      ~Mesh();

      void Render() const;

      void setIndices(const uint8 * _indices, uint _count);
      void setIndices(const uint16 * _indices, uint _count);
      void setIndices(const uint32 * _indices, uint _count);

      void AddVertices(const glm::vec3 * _vertices, uint _count);
      void AddUVs(const glm::vec2 * _uvs, uint _count);
      void AddNormals(const glm::vec3 * _normals, uint _count);

      template <typename T>
      void AddAttribute(
        const std::string & _name,
        const T * _data, uint _count, 
        bool _normalized = false
      );

      void Apply();

      void setDraw(DrawType _draw);

    protected:
      Mesh();

    private:
      void AddAttribute(
        const std::string & _name, 
        const void * _data, uint _size, 
        uint _count,
        Type _type, 
        ComponentType _component,
        uint _componentCount,
        bool _normalized = false
      );

      bool getAttribute(const std::string & _name, VertexAttribute * _outAttribute = nullptr) const;

      void ClearAttributes();

      std::unique_ptr<IndexBuffer> m_indices;
      std::unique_ptr<VertexArray> m_vao;

      DrawType m_draw;
      
      std::vector<glm::vec3> m_vertices;

      std::vector<VertexAttribute> m_attributes;
      std::vector<std::vector<byte>> m_data;
      uint m_elementCount;
      bool m_interleaved;


      std::vector<byte> m_indexData;
      IndexType m_indexType;
    };
  }
}

#include "Mesh.inl"

#endif //_ENGINE_GRAPHICS_MESH_H_