#include "stdafx.h"

#include "VertexArray.h"

namespace engine
{
  namespace graphics
  {
    VertexArray::VertexArray()
    { 
      GLCALL(glGenVertexArrays(1, &m_vao));
      Bind();
    }

    VertexArray::~VertexArray()
    {
      GLCALL(glDeleteVertexArrays(1, &m_vao));
    }

    void VertexArray::Bind() const
    {
      GLCALL(glBindVertexArray(m_vao));
    }

    void VertexArray::Unbind() const
    {
      GLCALL(glBindVertexArray(0));
    }

    void VertexArray::AddBuffer(const std::shared_ptr<VertexBuffer> _buffer)
    {
      _buffer->Bind();
      m_buffer.push_back(_buffer);
    }
  }
}