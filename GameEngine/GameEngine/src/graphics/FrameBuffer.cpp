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

  FrameBufferBase & FrameBuffer::BindDefault(FrameBufferBind _bind)
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
    glBlitFramebuffer(_srcX0, _srcY0, _srcX1, _srcY1, _dstX0, _dstY0, _dstX1, _dstY1, _mask, TextureFilterToOpenGL(_filter, false));
  }

  std::shared_ptr<FrameBuffer> FrameBuffer::Create(int _width, int _height, int _depth)
  {
    struct enable_fb : public FrameBuffer { };

    auto fb = std::make_shared<enable_fb>();
    fb->Bind();
    fb->Reset(_width, _height, _depth);
    return fb;
  }

  FrameBuffer::FrameBuffer() :
    m_fbo(0),
    m_clearFlags(0),
    m_depthStencil(false)
  {
    glGenFramebuffers(1, &m_fbo);
  }
  
  FrameBuffer::~FrameBuffer()
  {
    glDeleteFramebuffers(1, &m_fbo);

    if (Graphics::getContext().activeFrameBuffer.expired())
    {
      Graphics::getContext().activeFrameBuffer = Graphics::getContext().defaultFrameBuffer;
    }
  }
  
  void FrameBuffer::Bind(FrameBufferBind _bind)
  {
    glBindFramebuffer(FrameBufferBindToOpenGL(_bind), m_fbo);

    if (_bind == FrameBufferBind::WRITE)
    {
      glViewport(0, 0, m_width, m_height);
      Graphics::getContext().activeFrameBuffer = shared_from_this();
    }
  }
  
  void FrameBuffer::Clear()
  {
    glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
    glClear(m_clearFlags);
  }

  void FrameBuffer::Blit(GLenum _mask, TextureFilter _filter)
  {
    auto dst = Graphics::getContext().activeFrameBuffer.lock();
    Blit(
      0, 0, this->getWidth(), this->getHeight(),
      0, 0, dst->getWidth(), dst->getHeight(), 
      _mask, _filter
    );
  }

  void FrameBuffer::Attach(const std::shared_ptr<Texture2D> & _texture, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    assert(_texture);
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(*_texture, true, _attachment);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture->m_id,
      _level
    );

    Attach(*_texture, true, _texture, _attachment, _colourIndex);
  }

  void FrameBuffer::Attach(const std::shared_ptr<Texture2DArray> & _texture, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    assert(_texture);
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(*_texture, true, _attachment);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture->m_id,
      _level
    );

    Attach(*_texture, true, _texture, _attachment, _colourIndex);
  }

  void FrameBuffer::Attach(const std::shared_ptr<Texture2DArray> & _texture, int _layer, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    assert(_texture); 
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(*_texture, false, _attachment);

    glFramebufferTextureLayer(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture->m_id, 
      _level,
      _layer
    );

    Attach(*_texture, false, _texture, _attachment, _colourIndex);
  }

  void FrameBuffer::Attach(const std::shared_ptr<TextureCube> & _texture, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    assert(_texture);
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(*_texture, true, _attachment);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture->m_id,
      _level
    );

    Attach(*_texture, true, _texture, _attachment, _colourIndex);
  }

  void FrameBuffer::Attach(const std::shared_ptr<TextureCube> & _texture, int _layer, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    assert(_texture);
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(*_texture, false, _attachment);

    glFramebufferTextureLayer(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture->m_id, 
      _level,
      _layer
    );

    Attach(*_texture, false, _texture, _attachment, _colourIndex);
  }

  void FrameBuffer::AttachDepth(const std::shared_ptr<Shadow2D> & _texture, int _level)
  {
    assert(_texture);

    ValidateFrameBuffer(FrameBufferAttachment::DEPTH);
    ValidateTexture(*_texture, true, FrameBufferAttachment::DEPTH);

    glFramebufferTexture2D(
      GL_DRAW_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D,
      _texture->m_id,
      0
    );

    Attach(*_texture, true, _texture, FrameBufferAttachment::DEPTH);
  }

  void FrameBuffer::AttachDepth(const std::shared_ptr<Shadow2DArray> & _texture, int _level)
  {
    assert(_texture);

    ValidateFrameBuffer(FrameBufferAttachment::DEPTH);
    ValidateTexture(*_texture, true, FrameBufferAttachment::DEPTH);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      _texture->m_id,
      _level
    );

    Attach(*_texture, true, _texture, FrameBufferAttachment::DEPTH);
  }

  void FrameBuffer::AttachDepth(const std::shared_ptr<Shadow2DArray> & _texture, int _layer, int _level)
  {
    assert(_texture);

    ValidateFrameBuffer(FrameBufferAttachment::DEPTH);
    ValidateTexture(*_texture, false, FrameBufferAttachment::DEPTH);

    glFramebufferTextureLayer(
      GL_DRAW_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      _texture->m_id, 
      _level,
      _layer
    );

    Attach(*_texture, false, _texture, FrameBufferAttachment::DEPTH);
  }

  void FrameBuffer::AttachDepth(const std::shared_ptr<ShadowCube> & _texture, int _level)
  {
    assert(_texture);

    ValidateFrameBuffer(FrameBufferAttachment::DEPTH);
    ValidateTexture(*_texture, true, FrameBufferAttachment::DEPTH);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      _texture->m_id,
      _level
    );

    Attach(*_texture, true, _texture, FrameBufferAttachment::DEPTH);
  }

  void FrameBuffer::AttachDepth(const std::shared_ptr<ShadowCube> & _texture, int _layer, int _level)
  {
    assert(_texture);

    ValidateFrameBuffer(FrameBufferAttachment::DEPTH);
    ValidateTexture(*_texture, false, FrameBufferAttachment::DEPTH);

    glFramebufferTextureLayer(
      GL_DRAW_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      _texture->m_id,
      _level,
      _layer
    );

    Attach(*_texture, false, _texture, FrameBufferAttachment::DEPTH);
  }

  void FrameBuffer::AttachTemp(Texture2D & _texture, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(_texture, true, _attachment);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture.m_id,
      _level
    );

    Attach(_texture, true, nullptr, _attachment, _colourIndex);
  }

  void FrameBuffer::AttachTemp(Texture2DArray & _texture, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(_texture, true, _attachment);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture.m_id,
      _level
    );

    Attach(_texture, true, nullptr, _attachment, _colourIndex);
  }

  void FrameBuffer::AttachTemp(Texture2DArray & _texture, int _layer, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(_texture, false, _attachment);

    glFramebufferTextureLayer(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture.m_id, 
      _level,
      _layer
    );

    Attach(_texture, false, nullptr, _attachment, _colourIndex);
  }

  void FrameBuffer::AttachTemp(TextureCube & _texture, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(_texture, true, _attachment);

    glFramebufferTexture(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture.m_id,
      _level
    );

    Attach(_texture, true, nullptr, _attachment, _colourIndex);
  }

  void FrameBuffer::AttachTemp(TextureCube & _texture, int _layer, FrameBufferAttachment _attachment, int _colourIndex, int _level)
  {
    GetColourIndex(_colourIndex);

    ValidateFrameBuffer(_attachment, _colourIndex);
    ValidateTexture(_texture, false, _attachment);
    glFramebufferTexture2D(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + _layer, 
      _texture.m_id,
      _level
    );
    /*glFramebufferTextureLayer(
      GL_DRAW_FRAMEBUFFER,
      FrameBufferAttachmentToOpenGL(_attachment, _colourIndex),
      _texture.m_id, 0, _layer
    );*/

    Attach(_texture, false, nullptr, _attachment, _colourIndex);
  }

  /*void set(std::unique_ptr<RenderBuffer> _renderBuffer, FrameBufferAttachment _attachment, int _colourIndex)
  {

  }*/

  void FrameBuffer::ValidateFrameBuffer(FrameBufferAttachment _attachment, int _colourIndex)
  {
    if (Graphics::getContext().activeFrameBuffer.lock().get() != this)
    {
      //TODO: framebuffernotbound exception.
    }

    if (_attachment == FrameBufferAttachment::COLOUR &&
        _colourIndex >= Graphics::getContext().glData.getMaxColourAttachments())
    {
      throw std::out_of_range("maximum colour attachments reached");
    }
  }

  void FrameBuffer::ValidateTexture(Texture & _texture, bool _useLayers, FrameBufferAttachment _attachment)
  {
    if (_texture.getWidth()                      != m_width  ||
        _texture.getHeight()                     != m_height ||
        ((_useLayers) ? _texture.getDepth() : 1) != m_depth        
      )
    {
      throw std::invalid_argument("invalid texture size");
    }
    
    if (_attachment == FrameBufferAttachment::COLOUR)
    {
      TextureBaseFormat base = TextureFormatBase(_texture.getFormat());
      if (base != TextureBaseFormat::R   &&
          base != TextureBaseFormat::RG  &&
          base != TextureBaseFormat::RGB &&
          base != TextureBaseFormat::RGBA)
      {
        throw std::invalid_argument("invalid texture format");
      }
    }
    else if (_attachment == FrameBufferAttachment::DEPTH)
    {
      TextureBaseFormat base = TextureFormatBase(_texture.getFormat());
      if (base != TextureBaseFormat::DEPTH_COMPONENT &&
          base != TextureBaseFormat::DEPTH_STENCIL)
      {
        throw std::invalid_argument("invalid texture format");
      }
    }
    else if (_attachment == FrameBufferAttachment::STENCIL)
    {
      // TODO: can depth textures be stencil?
      TextureBaseFormat base = TextureFormatBase(_texture.getFormat());
      if (base != TextureBaseFormat::DEPTH_COMPONENT &&
        base != TextureBaseFormat::DEPTH_STENCIL)
      {
        throw std::invalid_argument("invalid texture format");
      }
    }
    else if (_attachment == FrameBufferAttachment::DEPTH_STENCIL)
    {
      if (TextureFormatBase(_texture.getFormat()) != TextureBaseFormat::DEPTH_STENCIL)
      {
        throw std::invalid_argument("invalid texture format");
      }
    }
    else { assert(false); }
  }

  void FrameBuffer::GetColourIndex(int & _index)
  {
    if (_index < 0)
    {
      _index = m_colourAttachments.size();
    }
  }

  void FrameBuffer::Attach(
    const Texture & _texture, bool _useLayers, 
    const std::shared_ptr<Texture> & _store,
    FrameBufferAttachment _attachment, int _colourIndex
  )
  {
    if (_attachment == FrameBufferAttachment::COLOUR)
    {
      m_colourAttachments[_colourIndex] = _store;

      std::vector<GLenum> buffers;
      for (auto & colour : m_colourAttachments)
      {
        buffers.push_back(GL_COLOR_ATTACHMENT0 + colour.first);
      }

      glDrawBuffers(buffers.size(), &buffers[0]);
   
      m_clearFlags |= BufferBit::COLOUR;
    }
    else if (_attachment == FrameBufferAttachment::DEPTH)
    {
      if (m_depthStencil)
      {
        m_stencilAttachment = nullptr;
      }

      m_depthAttachment = _store;

      m_depthStencil = false;
      m_clearFlags |= BufferBit::DEPTH;
    }
    else if (_attachment == FrameBufferAttachment::STENCIL)
    {
      if (m_depthStencil)
      {
        m_depthAttachment = nullptr;
      }

      m_stencilAttachment = _store;

      m_depthStencil = false;
      m_clearFlags |= BufferBit::STENCIL;
    }
    else if (_attachment == FrameBufferAttachment::DEPTH_STENCIL)
    {
      m_depthAttachment = _store;
      m_stencilAttachment = _store;

      m_depthStencil = true;
      m_clearFlags |= BufferBit::DEPTH | BufferBit::STENCIL;
    }

    Check();
  }
  
  void FrameBuffer::Check() const
  {
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      // TODO: custom exception
      throw std::runtime_error("invalid framebuffer");
    }
  }

  void FrameBuffer::Reset(int _width, int _height, int _depth)
  {
    for (auto & colour : m_colourAttachments)
    {
      glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colour.first, 0, 0);
    }

    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, 0, 0);

    m_width = _width;
    m_height = _height;
    m_depth = _depth;

    m_clearFlags = 0;

    m_colourAttachments.clear();
    m_depthAttachment = nullptr;
    m_stencilAttachment = nullptr;

    m_depthStencil = false;

    glViewport(0, 0, m_width, m_height);
  }

} } // engine::graphics