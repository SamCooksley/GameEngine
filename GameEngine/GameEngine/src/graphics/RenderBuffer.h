#ifndef _ENGINE_GRAPHICS_RENDERBUFFER_H_
#define _ENGINE_GRAPHICS_RENDERBUFFER_H_

#include "core\Types.h"
#include "opengl.h"
#include "TextureTypes.h"

namespace engine
{
  namespace graphics
  {
    class RenderBuffer
    {
      friend class FrameBuffer;

    public:
      RenderBuffer(uint _width, uint _height, TextureFormat _format);
      ~RenderBuffer();

      void Bind() const;
      void Unbind() const;

    private:
      GLuint m_rb;

      uint m_width, m_height;

      TextureFormat m_format;
    };
  }
}

#endif //_ENGINE_GRAPHICS_RENDERBUFFER_H_