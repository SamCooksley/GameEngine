#ifndef _ENGINE_GRAPHICS_RENDERBUFFER_H_
#define _ENGINE_GRAPHICS_RENDERBUFFER_H_

#include "core\Types.h"
#include "opengl.h"
#include "TextureTypes.h"

namespace engine {
namespace graphics {

  class RenderBuffer
  {
    friend class FrameBuffer;
  
   public:
    RenderBuffer(TextureFormat _format, int _width, int _height);
    ~RenderBuffer();
  
    void Bind() const;

    int getWidth() const;
    int getHeight() const;

    TextureFormat getFormat() const;
  
   private:
    GLuint m_rb;
  
    int m_width, m_height;
  
    TextureFormat m_format;
  };
  
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_RENDERBUFFER_H_