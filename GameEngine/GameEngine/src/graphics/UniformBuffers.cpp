#include "stdafx.h"

#include "UniformBuffers.h"

namespace engine
{
  namespace graphics
  {
    UniformBuffers::UniformBuffers()
    { }

    UniformBuffers::~UniformBuffers()
    { }

    void UniformBuffers::Add(std::unique_ptr<UniformBuffer> _buffer)
    {
      if (Exists(_buffer->getName()))
      {
        throw std::invalid_argument(
          "UniformBuffers Error: " + _buffer->getName() + " already exists"
        );
      }

      if (m_buffers.size() >= GL_MAX_UNIFORM_BUFFER_BINDINGS)
      {
        throw std::range_error(
          "UniformBuffers Error: maximum number of buffers reached: " + std::to_string(GL_MAX_UNIFORM_BUFFER_BINDINGS)
        );
      }

      _buffer->setBind(m_buffers.size());
      m_buffers[_buffer->getName()] = std::move(_buffer);
    }

    bool UniformBuffers::Exists(const std::string & _name) const
    {
      return m_buffers.find(_name) != m_buffers.end();
    }

    UniformBuffer & UniformBuffers::getBuffer(const std::string & _name)
    {
      auto buffer = m_buffers.find(_name);
      if (buffer == m_buffers.end())
      {
        throw std::out_of_range("UniformBuffers Error: " + _name + " does not exist.");
      }

      return *buffer->second;
    }
  }
}