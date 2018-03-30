#include "stdafx.h"

#include "VertexBuffer.h"

namespace engine
{
  namespace graphics
  {
    VertexBuffer::VertexBuffer() :
      m_vbo(0), m_size(0)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
    }

    VertexBuffer::~VertexBuffer()
    {
      GLCALL(glDeleteBuffers(1, &m_vbo));
    }

    void VertexBuffer::setData(const void * _data, uint _size)
    {
      GLCALL(glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW));
      m_size = _size;
    }

    void VertexBuffer::Bind() const
    {
      GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    }

    void VertexBuffer::Unbind() const
    {
      GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
  }
}