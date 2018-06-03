#ifndef _ENGINE_GRAPHICS_INDEXBUFFER_H_
#define _ENGINE_GRAPHICS_INDEXBUFFER_H_

#include "opengl.h"

#include "core\Types.h"

namespace engine {
namespace graphics {

  enum class DrawType : GLenum
  {
    POINTS         = GL_POINTS,
    LINES          = GL_LINES,
    LINE_STRIP     = GL_LINE_STRIP,
    LINE_LOOP      = GL_LINE_LOOP,
    TRIANGLES      = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN   = GL_TRIANGLE_FAN
  };
  
  GLenum DrawTypeToOpenGL(DrawType _type);
  DrawType OpenGLToDrawType(GLenum _type);
  
  enum class IndexType : GLenum
  {
    UNSIGNED_BYTE  = GL_UNSIGNED_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    UNSIGNED_INT   = GL_UNSIGNED_INT
  };
  
  GLenum IndexTypeToOpenGL(IndexType _type);
  IndexType OpenGLToIndexType(GLenum _type);
  
  class IndexBuffer : public NonCopyable
  {
   public:
    IndexBuffer(const uint8  * _data, uint _count);
    IndexBuffer(const uint16 * _data, uint _count);
    IndexBuffer(const uint32 * _data, uint _count);
    IndexBuffer(const void * _data, uint _size, IndexType _type);
  
    ~IndexBuffer();
  
    void Bind() const;
    void Unbind() const;
  
    void Draw(DrawType _draw) const;
  
   private:
    GLuint m_vbo;
    uint m_count;
    IndexType m_type;
  };
  
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_INDEXBUFFER_H_