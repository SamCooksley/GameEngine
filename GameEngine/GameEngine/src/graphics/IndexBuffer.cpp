#include "stdafx.h"

#include "IndexBuffer.h"

namespace engine
{
  namespace graphics
  {
    GLenum IndexTypeToOpenGL(IndexType _type)
    {
      return static_cast<GLenum>(_type);
    }

    IndexType OpenGLToIndexType(GLenum _type)
    {
      switch (_type)
      {
        case GL_UNSIGNED_BYTE:  { return IndexType::UNSIGNED_BYTE;  }
        case GL_UNSIGNED_SHORT: { return IndexType::UNSIGNED_SHORT; }
        case GL_UNSIGNED_INT:   { return IndexType::UNSIGNED_INT;   }
        default:
        {
          throw std::invalid_argument("Invalid index type");
        }
      }
    }

    GLenum DrawTypeToOpenGL(DrawType _type)
    {
      return static_cast<GLenum>(_type);
    }

    DrawType OpenGLToDrawType(GLenum _type)
    {
      switch (_type)
      {
        case GL_POINTS:         { return DrawType::POINTS;         }
        case GL_LINES:          { return DrawType::LINES;          }
        case GL_LINE_STRIP:     { return DrawType::LINE_STRIP;     }
        case GL_LINE_LOOP:      { return DrawType::LINE_LOOP;      }
        case GL_TRIANGLES:      { return DrawType::TRIANGLES;      }
        case GL_TRIANGLE_STRIP: { return DrawType::TRIANGLE_STRIP; }
        case GL_TRIANGLE_FAN:   { return DrawType::TRIANGLE_FAN;   }
        default:
        {
          throw std::invalid_argument("Invalid draw type.");
        }
      }
    }

    IndexBuffer::IndexBuffer(const uint8 * _data, uint _count) :
      m_vbo(0), m_count(_count), m_type(IndexType::UNSIGNED_BYTE)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint8), _data, GL_STATIC_DRAW));
    }

    IndexBuffer::IndexBuffer(const uint16 * _data, uint _count) :
      m_vbo(0), m_count(_count), m_type(IndexType::UNSIGNED_SHORT)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint16), _data, GL_STATIC_DRAW));
    }

    IndexBuffer::IndexBuffer(const uint32 * _data, uint _count) :
      m_vbo(0), m_count(_count), m_type(IndexType::UNSIGNED_INT)
    {
      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint32), _data, GL_STATIC_DRAW));
    }

    IndexBuffer::IndexBuffer(const void * _data, uint _size, IndexType _type) :
      m_vbo(0), m_count(0), m_type(_type)
    {
      switch (m_type)
      {
        case IndexType::UNSIGNED_BYTE:  { m_count = _size / sizeof(uint8);  break; }
        case IndexType::UNSIGNED_SHORT: { m_count = _size / sizeof(uint16); break; }
        case IndexType::UNSIGNED_INT:   { m_count = _size / sizeof(uint32); break; }
        default:
        {
          throw std::invalid_argument("Invalid index buffer type");
        }
      }

      GLCALL(glGenBuffers(1, &m_vbo));
      Bind();
      GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW));
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

    void IndexBuffer::Draw(DrawType _draw) const
    {
      GLCALL(glDrawElements(
        DrawTypeToOpenGL(_draw),
        m_count, 
        IndexTypeToOpenGL(m_type),
        reinterpret_cast<const void *>(0)
      ));
    }
  }
}