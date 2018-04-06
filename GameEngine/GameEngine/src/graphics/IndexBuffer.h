#ifndef _ENGINE_GRAPHICS_INDEXBUFFER_H_
#define _ENGINE_GRAPHICS_INDEXBUFFER_H_

#include "DrawType.h"

namespace engine
{
  namespace graphics
  {
    class IndexBuffer : public NonCopyable
    {
    public:
      IndexBuffer(const uint8 * _data, uint _count);
      IndexBuffer(const uint16 * _data, uint _count);
      IndexBuffer(const uint32 * _data, uint _count);
      IndexBuffer(const void * _data, uint _size, GLenum _type);

      ~IndexBuffer();

      void Bind() const;
      void Unbind() const;

      void Draw(DrawType::Type _draw) const;

    private:
      GLuint m_vbo;
      uint m_count;
      GLenum m_type;
    };
  }
}

#endif //_ENGINE_GRAPHICS_INDEXBUFFER_H_