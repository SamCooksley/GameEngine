#include "stdafx.h"

#include "UniformBuffer.h"

namespace engine {
namespace graphics {

  UniformBuffer::UniformBuffer(String _name) :
    m_ubo(0), m_index(GL_INVALID_INDEX), m_size(0),
    m_name(std::move(_name)) 
  {
    GLCALL(glGenBuffers(1, &m_ubo));
    Bind();
  }
  
  UniformBuffer::~UniformBuffer()
  {
    GLCALL(glDeleteBuffers(1, &m_ubo));
  }
  
  void UniformBuffer::Bind() const
  {
    GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ubo));
  }
  
  void UniformBuffer::Unbind() const
  {
    GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
  }
  
  void UniformBuffer::Reserve(uint _size)
  {
    setData(nullptr, _size);
  }
  
  void UniformBuffer::setData(const void * _data, uint _size)
  {
    m_size = _size;
    GLCALL(glBufferData(GL_UNIFORM_BUFFER, m_size, _data, GL_DYNAMIC_DRAW));
  }
  
  void UniformBuffer::setData(const void * _data, uint _size, uint _offset)
  {
    GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, _offset, _size, _data));
  }
  
  void UniformBuffer::setBind(GLuint _index)
  {
    m_index = _index;
    GLCALL(glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_ubo));
  }
  
  uint UniformBuffer::getBind() const
  {
    return m_index;
  }
  
  const String & UniformBuffer::getName() const
  {
    return m_name;
  }

} } // engine::graphics