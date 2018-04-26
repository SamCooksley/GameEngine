#include "stdafx.h"

#include "RenderBuffer.h"

namespace engine
{
  namespace graphics
  {
    RenderBuffer::RenderBuffer(uint _width, uint _height, TextureFormat _format) :
      m_rb(0), m_width(_width), m_height(_height), m_format(_format)
    {
      GLCALL(glGenRenderbuffers(1, &m_rb));
      Bind();

      GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, TextureFormatToOpenGL(_format), _width, _height));
    }

    RenderBuffer::~RenderBuffer()
    {
      GLCALL(glDeleteRenderbuffers(1, &m_rb));
    }

    void RenderBuffer::Bind() const
    {
      GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_rb));
    }

    void RenderBuffer::Unbind() const
    {
      GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }
  }
}