#ifndef _ENGINE_GRAPHICS_FRAMEBUFFER_H_
#define _ENGINE_GRAPHICS_FRAMEBUFFER_H_

#include "Texture2D.h"
#include "TextureCube.h"
#include "Shadow2D.h"

#include "RenderBuffer.h"

namespace engine {
namespace graphics {

  enum class FrameBufferAttachment : GLenum
  {
    COLOUR = GL_COLOR_ATTACHMENT0,
    DEPTH = GL_DEPTH_ATTACHMENT,
    STENCIL = GL_STENCIL_ATTACHMENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
  };
  
  GLenum FrameBufferAttachmentToOpenGL(FrameBufferAttachment _attachment, uint _colourOffset = 0);
  FrameBufferAttachment OpenGLToFrameBufferAttachment(GLenum _attachment);
  
  enum class FrameBufferBind : GLenum
  {
    FULL = GL_FRAMEBUFFER, //Binds to both
    READ = GL_READ_FRAMEBUFFER,
    WRITE = GL_DRAW_FRAMEBUFFER
  };

  GLenum FrameBufferBindToOpenGL(FrameBufferBind _bind);

  enum BufferBit : GLenum
  {
    COLOUR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT
  };

  class FrameBuffer : public std::enable_shared_from_this<FrameBuffer>
  {
   public:
    static FrameBuffer & BindDefault(FrameBufferBind _bind = FrameBufferBind::FULL);

    static void Blit(
      int _srcX0, int _srcY0, int _srcX1, int _srcY1,
      int _dstX0, int _dstY0, int _dstX1, int _dstY1,
      GLenum _mask,
      TextureFilter _filter = TextureFilter::NEAREST
    );

    static std::shared_ptr<FrameBuffer> Create(uint _width, uint _height);
    static std::shared_ptr<FrameBuffer> CreateDefault(uint _width, uint _height);

   public:
    ~FrameBuffer();
  
    void Bind(FrameBufferBind _bind = FrameBufferBind::FULL);
    void Unbind();
  
    void Clear();
  
    void Resize(uint _width, uint _height);

    void RenderToNDC() const; //TODO: move out of framebuffer

    void Blit(FrameBuffer & _dst, GLenum _mask, TextureFilter _filter = TextureFilter::NEAREST);

    std::shared_ptr<Texture2D> AddTexture(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type);
    
    std::shared_ptr<TextureCube> AddCubeMap(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type);
  
    std::shared_ptr<Shadow2D> AddShadow2D(TextureFormat _format);

    bool AddRenderBuffer(FrameBufferAttachment _attachment, TextureFormat _format);

    const std::shared_ptr<Texture2D> & getTexture(size_t _i);
    const std::shared_ptr<TextureCube> & getCubeMap(size_t _i);
    const std::shared_ptr<Shadow2D> & getShadow2D();

    uint getWidth() const;
    uint getHeight() const;
  
   protected:
    FrameBuffer(uint _width, uint _height, bool _default = false);

   private:
    bool Attach(FrameBufferAttachment _attachment);
  
    bool Check() const;
  
   private:
    GLuint m_fbo;
  
    glm::vec4 m_clearColour;
    GLenum m_clearFlags;
  
    uint m_width, m_height;
  
    uint m_colourAttachmentCount;
  
    std::vector<std::shared_ptr<Texture2D>> m_textures;
    std::vector<std::shared_ptr<TextureCube>> m_cubeMaps;
    std::shared_ptr<Shadow2D> m_shadow;

    std::vector<std::unique_ptr<RenderBuffer>> m_renderBuffers;
  };
  
} } // engine::graphics10

#endif //_ENGINE_GRAPHICS_FRAMEBUFFER_H_