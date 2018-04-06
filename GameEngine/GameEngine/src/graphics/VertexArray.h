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

      void AddBuffer(const std::shared_ptr<VertexBuffer> _buffer);

    private:
      GLuint m_vao;

      std::vector<std::shared_ptr<VertexBuffer>> m_buffer;
    };
  }
}

#endif //_ENGINE_GRAPHICS_VERTEXARRAY_H_