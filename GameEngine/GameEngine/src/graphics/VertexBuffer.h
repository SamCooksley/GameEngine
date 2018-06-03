#ifndef _ENGINE_GRAPHICS_VERTEXBUFFER_H_
#define _ENGINE_GRAPHICS_VERTEXBUFFER_H_

#include "opengl.h"
#include "BufferLayout.h"

namespace engine {
namespace graphics {

  class VertexBuffer : public NonCopyable
  {
    friend class VertexArray;

   public:
    VertexBuffer();
    ~VertexBuffer();
  
    void setData(const void * _data, uint _size);
  
    void Bind() const;
    void Unbind() const;
  
    void setLayout(const BufferLayout & _layout);
  
   private:
    void SetupLayout(uint _offset, bool _interleaved) const;
  
   private:
    GLuint m_vbo;
    uint m_size;
  
    BufferLayout m_layout;
  };
  
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_VERTEXBUFFER_H_