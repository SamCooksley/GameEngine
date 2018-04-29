#ifndef _ENGINE_GRAPHICS_VERTEXARRAY_H_
#define _ENGINE_GRAPHICS_VERTEXARRAY_H_

#include "VertexBuffer.h"

namespace engine
{
  namespace graphics
  {
    class VertexArray : public NonCopyable
    {
    public:
      VertexArray();
      ~VertexArray();

      void Bind() const;
      void Unbind() const;

      void AddBuffer(std::unique_ptr<VertexBuffer> _buffer, bool _interleaved);

    private:
      GLuint m_vao;

      std::vector<std::unique_ptr<VertexBuffer>> m_buffer;
      uint m_locationCount;
    };
  }
}

#endif //_ENGINE_GRAPHICS_VERTEXARRAY_H_