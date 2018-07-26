#ifndef _ENGINE_GRAPHICS_FRAMEBUFFER_H_
#define _ENGINE_GRAPHICS_FRAMEBUFFER_H_

#include "FrameBufferBase.h"

#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureCube.h"

#include "Shadow2D.h"
#include "Shadow2DArray.h"
#include "ShadowCube.h"

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

  class FrameBuffer : public FrameBufferBase
  {
   public:
     FrameBufferBase & BindDefault(FrameBufferBind _bind = FrameBufferBind::WRITE);

    static void Blit(
      int _srcX0, int _srcY0, int _srcX1, int _srcY1,
      int _dstX0, int _dstY0, int _dstX1, int _dstY1,
      GLenum _mask,
      TextureFilter _filter = TextureFilter::NEAREST
    );

    static std::shared_ptr<FrameBuffer> Create(int _width, int _height, int _depth = 1);

   public:
    ~FrameBuffer();
  
    void Bind(FrameBufferBind _bind = FrameBufferBind::WRITE) override;
  
    void Clear() override;

    void Blit(GLenum _mask, TextureFilter _filter = TextureFilter::NEAREST);
    
    void Attach(const std::shared_ptr<Texture2D> & _texture, FrameBufferAttachment _attachment, int _colourIndex = -1);
    void Attach(const std::shared_ptr<Texture2DArray> & _texture, FrameBufferAttachment _attachement, int _colourIndex = -1);
    void Attach(const std::shared_ptr<Texture2DArray> & _texture, int _layer, FrameBufferAttachment _attachment, int _colourIndex = -1);
    
    void Attach(const std::shared_ptr<TextureCube> & _texture, FrameBufferAttachment _attachment, int _colourIndex = -1);
    void Attach(const std::shared_ptr<TextureCube> & _texture, int _layer, FrameBufferAttachment _attachment, int _colourInidex = -1);

    void AttachDepth(const std::shared_ptr<Shadow2D> & _shadow);
    void AttachDepth(const std::shared_ptr<Shadow2DArray> & _shadow);
    void AttachDepth(const std::shared_ptr<Shadow2DArray> & _shadow, int _layer);
    void AttachDepth(const std::shared_ptr<ShadowCube> & _shadow);
    void AttachDepth(const std::shared_ptr<ShadowCube> & _shadow, int _layer);

    void AttachTemp(Texture2D & _texture, FrameBufferAttachment _attachment, int _colourIndex = 0);
    void AttachTemp(Texture2DArray & _texture, FrameBufferAttachment _attachement, int _colourIndex = 0);
    void AttachTemp(Texture2DArray & _texture, int _layer, FrameBufferAttachment _attachment, int _colourIndex = 0);
    
    void AttachTemp(TextureCube & _texture, FrameBufferAttachment _attachment, int _colourIndex = 0);
    void AttachTemp(TextureCube & _texture, int _layer, FrameBufferAttachment _attachement, int _colourIndex = 0);

    //void set(std::unique_ptr<RenderBuffer> _renderBuffer, FrameBufferAttachment _attachment, int _colourIndex = -1);

    void Reset(int _width, int _height, int _depth = 1);

    template <class T>
    std::shared_ptr<T> getColourAttachment(int _i);
    template <>
    std::shared_ptr<Texture> getColourAttachment(int _i);

    template <class T>
    std::shared_ptr<T> getDepthAttachment();
    template <>
    std::shared_ptr<Texture> getDepthAttachment();

    template <class T>
    std::shared_ptr<T> getStencilAttachment();
    template <>
    std::shared_ptr<Texture> getStencilAttachment();
  
   protected:
    FrameBuffer();

   private:
    void ValidateFrameBuffer(FrameBufferAttachment _attachment, int _colourIndex = 0);
    void ValidateTexture(Texture & _texture, bool _useLayers, FrameBufferAttachment _attachment);

    void GetColourIndex(int & _index);

    void Attach(
      const Texture & _texture, bool _useLayers,
      const std::shared_ptr<Texture> & _store,
      FrameBufferAttachment _attachment,
      int _colourIndex = 0
    );
  
    void Check() const;
  
   private:
    GLuint m_fbo;
  
    GLenum m_clearFlags;
  
    bool m_depthStencil;

    std::map<int, std::shared_ptr<Texture>> m_colourAttachments;

    //std::vector<std::shared_ptr<Texture>> m_colourAttachments;
    std::shared_ptr<Texture> m_depthAttachment;
    std::shared_ptr<Texture> m_stencilAttachment;
    

    //std::vector<std::unique_ptr<RenderBuffer>> m_renderBuffers;
  };
  
} } // engine::graphics

#include "FrameBuffer.inl"

#endif //_ENGINE_GRAPHICS_FRAMEBUFFER_H_