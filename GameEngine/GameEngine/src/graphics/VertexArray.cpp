#include "stdafx.h"

#include "VertexArray.h"

namespace engine
{
  namespace graphics
  {
    VertexArray::VertexArray() :
      m_locationCount(0)
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

    void VertexArray::AddBuffer(std::unique_ptr<VertexBuffer> _buffer, bool _interleaved)
    {
      assert(_buffer);

      _buffer->Bind();
      _buffer->SetupLayout(m_locationCount, _interleaved);
      m_locationCount += _buffer->m_layout.getElementCount();

      m_buffer.push_back(std::move(_buffer));
    }
  }
}