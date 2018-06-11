#include "stdafx.h"

#include "FrameBuffer.h"

#include "Graphics.h"
#include "Screen.h"

namespace engine {
namespace graphics {

  GLenum FrameBufferAttachmentToOpenGL(FrameBufferAttachment _attachment, uint _colourOffset)
  {
    GLenum att = static_cast<GLenum>(_attachment);
    if (_attachment == FrameBufferAttachment::COLOUR)
    {
      att += _colourOffset;
    }
    return att;
  }
  
  FrameBufferAttachment OpenGLToFrameBufferAttachment(GLenum _attachment)
  {
    if (_attachment >= GL_COLOR_ATTACHMENT0 && _attachment < GL_DEPTH_ATTACHMENT)
    {
      return FrameBufferAttachment::COLOUR;
    }
  
    switch (_attachment)
    {
      case GL_DEPTH_ATTACHMENT:         { return FrameBufferAttachment::DEPTH;         }
      case GL_STENCIL_ATTACHMENT:       { return FrameBufferAttachment::STENCIL;       }
      case GL_DEPTH_STENCIL_ATTACHMENT: { return FrameBufferAttachment::DEPTH_STENCIL; }
      default: 
      {
        throw std::invalid_argument("Invalid buffer attachment");
      }
    }
  
    return FrameBufferAttachment::COLOUR;
  }

  GLenum FrameBufferBindToOpenGL(FrameBufferBind _bind)
  {
    return static_cast<GLenum>(_bind);
  }

  FrameBuffer & FrameBuffer::BindDefault(FrameBufferBind _bind)
  {
    Graphics::getContext().defaultFrameBuffer->Bind(_bind);
    return *Graphics::getContext().defaultFrameBuffer;
  }

  void FrameBuffer::Blit(
    int _srcX0, int _srcY0, int _srcX1, int _srcY1,
    int _dstX0, int _dstY0, int _dstX1, int _dstY1,
    GLenum _mask,
    TextureFilter _filter
  )
  {
    GLCALL(glBlitFramebuffer(_srcX0, _srcY0, _srcX1, _srcY1, _dstX0, _dstY0, _dstX1, _dstY1, _mask, TextureFilterToOpenGL(_filter)));
  }

  std::shared_ptr<FrameBuffer> FrameBuffer::Create(uint _width, uint _height)
  {
    struct enable_fb : public FrameBuffer 
    { 
      enable_fb(uint _width, uint _height) : FrameBuffer(_width, _height, false) 
      { } 
    };
    auto fb = std::make_shared<enable_fb>(_width, _height);
    fb->Bind();
    return fb;
  }

  std::shared_ptr<FrameBuffer> FrameBuffer::CreateDefault(uint _width, uint _height)
  {
    struct enable_fb : public FrameBuffer
    {
      enable_fb(uint _width, uint _height) : FrameBuffer(_width, _height, true) 
      { }
    };
    auto fb = std::make_shared<enable_fb>(_width, _height);
    fb->Bind();
    return fb;
  }

  FrameBuffer::FrameBuffer(uint _width, uint _height, bool _default) :
    m_fbo(0),
    m_clearColour(0.f, 0.f, 0.f, 1.f),
    m_clearFlags(0),
    m_width(_width), m_height(_height),
    m_colourAttachmentCount(0)
  {
    if (!_default)
    {
      GLCALL(glGenFramebuffers(1, &m_fbo));
    }
    else
    {
      assert(!Graphics::getContext().defaultFrameBuffer && "Default framebuffer already exists");
      m_clearFlags = BufferBit::COLOUR | BufferBit::DEPTH;
    }
  }
  
  FrameBuffer::~FrameBuffer()
  {
    GLCALL(glDeleteFramebuffers(1, &m_fbo));
  }
  
  void FrameBuffer::Bind(FrameBufferBind _bind)
  {
    if (Graphics::getContext().activeFrameBuffer.lock().get() != this)
    {
      GLCALL(glBindFramebuffer(FrameBufferBindToOpenGL(_bind), m_fbo));
      GLCALL(glViewport(0, 0, m_width, m_height));

      Graphics::getContext().activeFrameBuffer = shared_from_this();
    }
  }
  
  void FrameBuffer::Unbind()
  {
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    Graphics::getContext().activeFrameBuffer = Graphics::getContext().defaultFrameBuffer;
  }
  
  void FrameBuffer::Clear()
  {
    GLCALL(glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a));
    GLCALL(glClear(m_clearFlags));
  }

  void FrameBuffer::Resize(uint _width, uint _height)
  {
    if (_width == m_width && _height == m_height) { return; }

    m_width = _width;
    m_height = _height;

    for (auto & tex : m_textures)
    {
      tex->Bind(0);
      tex->Resize(m_width, m_height);
    }

    for (auto & rb : m_renderBuffers)
    {
      rb->Bind();
      rb->Resize(m_width, m_height);
    }
  }

  void FrameBuffer::RenderToNDC() const
  {
    Graphics::getContext().screenQuad->Render();
  }

  void FrameBuffer::Blit(FrameBuffer & _dst, GLenum _mask, TextureFilter _filter)
  {
    Blit(
      0, 0, this->getWidth(), this->getHeight(),
      0, 0, _dst.getWidth(), _dst.getHeight(), 
      _mask, _filter
    );
  }
  
  std::shared_ptr<Texture2D> FrameBuffer::AddTexture(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type)
  {
    assert(m_fbo != 0 && "Cannot attach items to default framebuffer");

    auto texture = Texture2D::Create(m_width, m_height, _format, _type);
    texture->setFilter(TextureFilter::LINEAR);
    texture->setWrap(TextureWrap::CLAMP_TO_EDGE);
  
    GLCALL(glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        FrameBufferAttachmentToOpenGL(_attachment, m_colourAttachmentCount),
        GL_TEXTURE_2D,
        texture->m_id,
        0
    ));
  
    if (!Attach(_attachment))
    {
      return nullptr;
    }
  
    m_textures.push_back(texture);
  
    return texture;
  }
  
  std::shared_ptr<TextureCube> FrameBuffer::AddCubeMap(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type)
  {
    assert(m_fbo != 0 && "Cannot attach items to default framebuffer");

    auto cube = std::make_shared<TextureCube>(m_width, m_height, _format, _type);
    cube->setFilter(TextureFilter::LINEAR);
  
    GLCALL(glFramebufferTexture(
        GL_FRAMEBUFFER,
        FrameBufferAttachmentToOpenGL(_attachment, m_colourAttachmentCount),
        cube->m_id,
        0
    ));
  
    if (!Attach(_attachment))
    {
      return nullptr;
    }
  
    m_cubeMaps.push_back(cube);
  
    return cube;
  }

  std::shared_ptr<Shadow2D> FrameBuffer::AddShadow2D(TextureFormat _format)
  {
    assert(m_fbo != 0 && "Cannot attach items to default framebuffer");
    assert(!m_shadow && "FrameBuffer already has a shadow texture");

    auto texture = Shadow2D::Create(m_width, m_height, _format);

    GLCALL(glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(FrameBufferAttachment::DEPTH, m_colourAttachmentCount),
      GL_TEXTURE_2D,
      texture->m_id,
      0
    ));

    if (!Attach(FrameBufferAttachment::DEPTH))
    {
      return nullptr;
    }

    m_shadow = texture;

    return texture;
  }
  
  bool FrameBuffer::AddRenderBuffer(FrameBufferAttachment _attachment, TextureFormat _format)
  {
    assert(m_fbo != 0 && "Cannot attach items to default framebuffer");

    auto renderBuffer = std::make_unique<RenderBuffer>(m_width, m_height, _format);
  
    GLCALL(glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        FrameBufferAttachmentToOpenGL(_attachment, m_colourAttachmentCount),
        GL_RENDERBUFFER,
        renderBuffer->m_rb
    ));
  
    if (!Attach(_attachment))
    {
      return false;
    }
  
    m_renderBuffers.push_back(std::move(renderBuffer));
  
    return true;
  }
  
  bool FrameBuffer::Attach(FrameBufferAttachment _attachment)
  {
    //make the frame buffer clear attachmented types.
    if (_attachment == FrameBufferAttachment::COLOUR)
    {
      if (m_colourAttachmentCount >= Graphics::GL().GetMaxColourAttachments())
      {
        throw std::runtime_error("Maximum colour attachments reached");
      }
  
      ++m_colourAttachmentCount;
  
      //draw all the colour buffers.
      std::vector<GLenum> buffers(m_colourAttachmentCount);
      for (uint i = 0; i < m_colourAttachmentCount; ++i)
      {
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;
      }
  
      GLCALL(glDrawBuffers(buffers.size(), &buffers[0]));
  
      m_clearFlags |= BufferBit::COLOUR;
    }
    else if (_attachment == FrameBufferAttachment::DEPTH)
    {
      m_clearFlags |= BufferBit::DEPTH;
    }
    else if (_attachment == FrameBufferAttachment::STENCIL)
    {
      m_clearFlags |= BufferBit::STENCIL;
    }
    else if (_attachment == FrameBufferAttachment::DEPTH_STENCIL)
    {
      m_clearFlags |= BufferBit::DEPTH | BufferBit::STENCIL;
    }
  
    return Check();
  }
  
  bool FrameBuffer::Check() const
  {
    GLCALL(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    //if it is not complete, log an error.
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      debug::LogError("Failed to create FrameBuffer: " + std::to_string(status));
    }
  
    return status == GL_FRAMEBUFFER_COMPLETE;
  }
  
  const std::shared_ptr<Texture2D> & FrameBuffer::getTexture(size_t _i)
  {
    return m_textures.at(_i);
  }

  const std::shared_ptr<TextureCube> & FrameBuffer::getCubeMap(size_t _i)
  {
    return m_cubeMaps.at(_i);
  }

  const std::shared_ptr<Shadow2D> & FrameBuffer::getShadow2D()
  {
    return m_shadow;
  }

  uint FrameBuffer::getWidth() const
  {
    return m_width;
  }
  
  uint FrameBuffer::getHeight() const
  {
    return m_height;
  }

} } // engine::graphics