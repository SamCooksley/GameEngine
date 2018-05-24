#include "stdafx.h"

#include "FrameBuffer.h"

namespace engine
{
  namespace graphics
  {
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
      if (_attachment >= GL_COLOR_ATTACHMENT0 && _attachment <= GL_COLOR_ATTACHMENT15)
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

    FrameBuffer::FrameBuffer(uint _width, uint _height) :
      m_fbo(0),
      m_clearColour(0.f, 0.f, 0.f, 1.f),
      m_clearFlags(0),
      m_width(_width), m_height(_height),
      m_colourAttachmentCount(0)
    {
      GLCALL(glGenFramebuffers(1, &m_fbo));
    }

    FrameBuffer::~FrameBuffer()
    {
      GLCALL(glDeleteFramebuffers(1, &m_fbo));
    }

    void FrameBuffer::Bind()
    {
      GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
      GLCALL(glViewport(0, 0, m_width, m_height));
    }

    void FrameBuffer::Unbind()
    {
      GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void FrameBuffer::Clear()
    {
      GLCALL(glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a));
      GLCALL(glClear(m_clearFlags));
    }

    std::shared_ptr<Texture2D> FrameBuffer::AddTexture(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type)
    {
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

      return std::move(texture);
    }

    std::shared_ptr<TextureCube> FrameBuffer::AddCubeMap(FrameBufferAttachment _attachment, TextureFormat _format, TextureDataType _type)
    {
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

      m_textures.push_back(cube);

      return std::move(cube);
    }

    bool FrameBuffer::AddRenderBuffer(FrameBufferAttachment _attachment, TextureFormat _format)
    {
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
      //if it was a colour attachment, increase the colour count.
      if (_attachment == FrameBufferAttachment::COLOUR)
      {
        ++m_colourAttachmentCount;
      }

      //make the frame buffer clear attachmented types.
      if (_attachment == FrameBufferAttachment::COLOUR)
      {
        //draw all the colour buffers.
        std::vector<GLenum> buffers(m_colourAttachmentCount);
        for (uint i = 0; i < m_colourAttachmentCount; ++i)
        {
          buffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        GLCALL(glDrawBuffers(buffers.size(), &buffers[0]));

        m_clearFlags |= GL_COLOR_BUFFER_BIT;
      }
      else if (_attachment == FrameBufferAttachment::DEPTH)
      {
        m_clearFlags |= GL_DEPTH_BUFFER_BIT;
      }
      else if (_attachment == FrameBufferAttachment::STENCIL)
      {
        m_clearFlags |= GL_STENCIL_BUFFER_BIT;
      }
      else if (_attachment == FrameBufferAttachment::DEPTH_STENCIL)
      {
        m_clearFlags |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
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

    uint FrameBuffer::getWidth() const
    {
      return m_width;
    }

    uint FrameBuffer::getHeight() const
    {
      return m_height;
    }
  }
}