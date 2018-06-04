#include "stdafx.h"

#include "RenderBuffer.h"

namespace engine {
namespace graphics {

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

  void RenderBuffer::Resize(uint _width, uint _height)
  {
    if (_width == m_width && _height == m_height) { return; }

    m_width = _width;
    m_height = _height;

    GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, TextureFormatToOpenGL(m_format), m_width, m_height));
  }

} } // engine::graphics