#include "stdafx.h"

#include "RenderBuffer.h"

namespace engine {
namespace graphics {

  RenderBuffer::RenderBuffer(TextureFormat _format, int _width, int _height) :
    m_rb(0), m_width(_width), m_height(_height), m_format(_format)
  {
    glGenRenderbuffers(1, &m_rb);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rb);
  
    glRenderbufferStorage(GL_RENDERBUFFER, TextureFormatToOpenGL(_format), _width, _height);
  }
  
  RenderBuffer::~RenderBuffer()
  {
    glDeleteRenderbuffers(1, &m_rb);
  }
  
  void RenderBuffer::Bind() const
  {
    glBindRenderbuffer(GL_RENDERBUFFER, m_rb);
  }

  int RenderBuffer::getWidth() const
  {
    return m_width;
  }

  int RenderBuffer::getHeight() const
  {
    return m_height;
  }

  TextureFormat RenderBuffer::getFormat() const
  {
    return m_format;
  }

} } // engine::graphics