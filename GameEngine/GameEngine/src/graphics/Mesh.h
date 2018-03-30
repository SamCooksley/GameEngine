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
    };

    class Mesh : public core::Object
    {
    public:
      static std::shared_ptr<Mesh> Load(const std::string & _path);

      ~Mesh();

      void Render() const;
      void Render(Shader & _shader) const;

      void setIndices(const uint16 * _indices, uint _count);

    protected:
      Mesh();

    private:
      void AddAttribute(const std::string & _name, const void * _data, uint _size);

      std::unique_ptr<IndexBuffer> m_indices;
      std::unique_ptr<VertexBuffer> m_vbo;

      DrawType::Type m_draw;

      std::vector<VertexAttribute> m_attributes;
      std::map<std::string, size_t> m_nameToAttribute;

      std::vector<glm::vec3> m_vertices;

      std::vector<byte> m_vboData;
      uint m_vertexCount;

      std::vector<uint16> m_indexData;
    };
  }
}

#endif //_ENGINE_GRAPHICS_MESH_H_