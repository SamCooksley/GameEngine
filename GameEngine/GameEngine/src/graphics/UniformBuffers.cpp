#include "stdafx.h"

#include "UniformBuffers.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  UniformBuffers::UniformBuffers()
  { }
  
  UniformBuffers::~UniformBuffers()
  { }
  
  void UniformBuffers::Add(std::unique_ptr<UniformBuffer> _buffer)
  {
    assert(_buffer);
  
    if (getBuffer(_buffer->getName()) != nullptr)
    {
      throw std::invalid_argument(
        "Uniform buffer already exists"
      );
    }
  
    //TODO: GL_MAX_UNIFORM_BUFFER_BINDINGS
    if (m_buffers.size() >= GL_MAX_UNIFORM_BUFFER_BINDINGS)
    {
      throw std::range_error(
        "Maximum number of uniform buffers reached"
      );
    }
  
    _buffer->setBind(m_buffers.size());
    m_buffers[_buffer->getName()] = std::move(_buffer);
  
    //add the new uniform buffer to all the shaders. 
    for (auto & shader : Graphics::getContext().shaders)
    {
      shader.lock()->SetupUniformBuffer(*_buffer);
    }
  }
  
  UniformBuffer * UniformBuffers::getBuffer(const String & _name)
  {
    auto buffer = m_buffers.find(_name);
    if (buffer == m_buffers.end())
    {
      return nullptr;
    }
  
    return buffer->second.get();
  }

} } // engine::graphics