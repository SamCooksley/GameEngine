#ifndef _ENGINE_GRAPHICS_MESH_H_
#define _ENGINE_GRAPHICS_MESH_H_

#include "core\Object.h"

#include "VertexBuffer.h"
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
      uint offset;
      uint size;
      GLenum type;
      uint count;
      bool normalized;
    };

    class Mesh : public core::Object
    {
    public:
      static std::shared_ptr<Mesh> Load(const std::string & _path);

      ~Mesh();

      void Render() const;
      void Render(Shader & _shader) const;

      void setIndices(const uint8 * _indices, uint _count);
      void setIndices(const uint16 * _indices, uint _count);
      void setIndices(const uint32 * _indices, uint _count);

      void setVertices(const glm::vec3 * _vertices, uint _count);
      void setUVs(const glm::vec2 * _uvs, uint _count);
      void setNormals(const glm::vec3 * _normals, uint _count);

      template <typename T>
      void setAttribute(
        const std::string & _name,
        const T * _data, uint _count, 
        bool _normalized = false
      );

      void RemoveAttribute(const std::string & _name);

      void Apply();

    protected:
      Mesh();

    private:
      void AddAttribute(
        const std::string & _name, 
        const void * _data, uint _size, 
        uint _count,
        GLenum _componentType, uint _componentCount, 
        bool _normalized = false
      );

      void setAttribute(
        const std::string & _name,
        const void * _data, uint _size, 
        uint _count, 
        GLenum _componentType, uint _componentCount, 
        bool _normalized = false
      );

      bool getAttribute(const std::string & _name, VertexAttribute * _outAttribute = nullptr) const;

      void ClearAttributes();

      std::unique_ptr<IndexBuffer> m_indices;
      std::unique_ptr<VertexBuffer> m_vbo;

      DrawType::Type m_draw;
      
      std::vector<glm::vec3> m_vertices;

      std::vector<VertexAttribute> m_attributes;
      std::map<std::string, size_t> m_nameToAttribute;

      std::vector<byte> m_vboData;

      std::vector<byte> m_indexData;
      GLenum m_indexType;
    };
  }
}

#include "Mesh.inl"

#endif //_ENGINE_GRAPHICS_MESH_H_