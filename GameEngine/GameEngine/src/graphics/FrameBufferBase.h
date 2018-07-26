#ifndef _ENGINE_GRAPHICS_FRAMEBUFFERBASE_H_
#define _ENGINE_GRAPHICS_FRAMEBUFFERBASE_H_

#include "opengl.h"
#include "TextureTypes.h"

namespace engine {
namespace graphics {

  enum class FrameBufferBind : GLenum
  {
    READ = GL_READ_FRAMEBUFFER,
    WRITE = GL_DRAW_FRAMEBUFFER
  };

  GLenum FrameBufferBindToOpenGL(FrameBufferBind _bind);

  enum BufferBit : GLenum
  {
    COLOUR  = GL_COLOR_BUFFER_BIT,
    DEPTH   = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT
  };

  class FrameBufferBase : public std::enable_shared_from_this<FrameBufferBase>
  {
   public:
    FrameBufferBase();
    virtual ~FrameBufferBase();

    virtual void Bind(FrameBufferBind _bind = FrameBufferBind::WRITE) = 0;

    virtual void Clear() = 0;

    int getWidth() const;
    int getHeight() const;
    int getDepth() const;

    void setClearColour(const glm::vec4 & _colour);

  protected:
    int m_width, m_height, m_depth;

    glm::vec4 m_clearColour;
  };

} } // engine::graphics 

#endif // _ENGINE_GRAPHICS_FRAMEBUFFERBASE_H_