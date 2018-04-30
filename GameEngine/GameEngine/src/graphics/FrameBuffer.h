#ifndef _ENGINE_GRAPHICS_FRAMEBUFFER_H_
#define _ENGINE_GRAPHICS_FRAMEBUFFER_H_

#include "Texture2D.h"
#include "TextureCube.h"

#include "RenderBuffer.h"

namespace engine
{
  namespace graphics
  {
    enum class FrameBufferAttachment : GLenum
    {
      COLOUR = GL_COLOR_ATTACHMENT0,
      DEPTH = GL_DEPTH_ATTACHMENT,
      STENCIL = GL_STENCIL_ATTACHMENT,
      DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };

    GLenum FrameBufferAttachmentToOpenGL(FrameBufferAttachment _attachment, uint _colourOffset = 0);
    FrameBufferAttachment OpenGLToFrameBufferAttachment(GLenum _attachment);

    class FrameBuffer
    {
    public:
      FrameBuffer(uint _width, uint _height);
      ~FrameBuffer();

      void Bind();
      void Unbind();

      void Clear();

      std::shared_ptr<Texture2D> AddTexture(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type);
      
      std::shared_ptr<TextureCube> AddCubeMap(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type);

      bool AddRenderBuffer(FrameBufferAttachment _attachment, TextureFormat _format);

      uint getWidth() const;
      uint getHeight() const;

    private:
      bool Attach(FrameBufferAttachment _attachment);

      bool Check() const;

      GLuint m_fbo;

      glm::vec4 m_clearColour;
      GLenum m_clearFlags;

      uint m_width, m_height;

      uint m_colourAttachmentCount;

      std::vector<std::shared_ptr<Texture>> m_textures;
      std::vector<std::unique_ptr<RenderBuffer>> m_renderBuffers;
    };
  }
}

#endif //_ENGINE_GRAPHICS_FRAMEBUFFER_H_