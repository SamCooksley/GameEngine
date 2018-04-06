#ifndef _ENGINE_GRAPHICS_VERTEXBUFFER_H_
#define _ENGINE_GRAPHICS_VERTEXBUFFER_H_

#include "core\Types.h"
#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    class VertexBuffer : public NonCopyable
    {
    public:
      VertexBuffer();
      ~VertexBuffer();

      void setData(const void * _data, uint _size);

      void Bind() const;
      void Unbind() const;

    private:
      GLuint m_vbo;
      uint m_size;
    };
  }
}

#endif //_ENGINE_GRAPHICS_VERTEXBUFFER_H_