#include "stdafx.h"

#include "IndexBuffer.h"

namespace engine
{
  namespace graphics
  {
    IndexBuffer::IndexBuffer(const uint8 * _data, uint _count) :
      m_vbo(0), m_count(_count), m_type(GL_UNSIGNED_BYTE)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint8), _data, GL_STATIC_DRAW));
    }

    IndexBuffer::IndexBuffer(const uint16 * _data, uint _count) :
      m_vbo(0), m_count(_count), m_type(GL_UNSIGNED_SHORT)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint16), _data, GL_STATIC_DRAW));
    }

    IndexBuffer::IndexBuffer(const uint32 * _data, uint _count) :
      m_vbo(0), m_count(_count), m_type(GL_UNSIGNED_INT)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint32), _data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
      GLCALL(glDeleteBuffers(1, &m_vbo));
    }

    void IndexBuffer::Bind() const
    {
      GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo));
    }

    void IndexBuffer::Unbind() const
    {
      GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::Draw(DrawType::Type _draw) const
    {
      GLCALL(glDrawElements(DrawType::ToOpenGL(_draw), m_count, m_type, (const void*)0));
    }
  }
}