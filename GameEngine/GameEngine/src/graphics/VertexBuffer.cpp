#include "stdafx.h"

#include "VertexBuffer.h"

#include "debug\Debug.h"

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

    void VertexBuffer::setLayout(const BufferLayout & _layout)
    {
      m_layout = _layout;
    }

    void VertexBuffer::SetupLayout(uint _offset, bool _interleaved) const
    {
      for (size_t i = 0; i < m_layout.getElementCount(); ++i)
      {
        auto & element = m_layout.at(i);

        GLuint loc = i + _offset;

        GLCALL(glEnableVertexAttribArray(loc));
        GLCALL(glVertexAttribPointer(
          loc, 
          element.count,
          ComponentTypeToOpenGL(element.type), 
          element.normalized, 
          _interleaved? m_layout.getSize() : 0, 
          reinterpret_cast<const void *>(element.offset)
        ));
      }
    }
  }
}