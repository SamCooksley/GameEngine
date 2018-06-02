#include "stdafx.h"

#include "CommandBuffer.h"

namespace engine
{
  namespace graphics
  {
    CommandBuffer::CommandBuffer()
    { }

    CommandBuffer::~CommandBuffer()
    { }

    void CommandBuffer::Add(
      const std::shared_ptr<Mesh> & _mesh,
      const std::shared_ptr<Material> & _material,
      const glm::mat4 & _transform
    )
    {
      assert(_mesh && _material);

      std::vector<Command> * buffer = nullptr;

      switch (_material->getShader()->getRenderQueue())
      { 
        case RenderQueue::DEFERRED:    { buffer = &m_deferred;    break; }
        case RenderQueue::FORWARD:     { buffer = &m_forward;     break; }
        case RenderQueue::TRANSPARENT: { buffer = &m_transparent; break; }
      }

      assert(buffer != nullptr);

      buffer->push_back({ _mesh, _material, _transform });
    }

    void CommandBuffer::Clear()
    {
      m_deferred.clear();
      m_forward.clear();
      m_transparent.clear();
    }

    void CommandBuffer::Sort(const Camera & _camera)
    {
      auto sort = [](const Command & _lhs, const Command & _rhs) -> bool
      {
        if (_lhs.material->getShader() == _rhs.material->getShader())
        {
          return _lhs.material < _rhs.material;
        }
        return _lhs.material->getShader() < _rhs.material->getShader();
      };

      auto sortTrans = [&_camera](const Command & _lhs, const Command & _rhs) -> bool
      {
        return
          _lhs.mesh->getBounds().Multiply(_lhs.transform).Distance2(_camera.position) >
          _rhs.mesh->getBounds().Multiply(_rhs.transform).Distance2(_camera.position);        
      };
      
      std::sort(m_deferred.begin(), m_deferred.end(), sort);
      std::sort(m_forward.begin(), m_forward.end(), sort);
      std::sort(m_transparent.begin(), m_transparent.end(), sortTrans);
    }

    const std::vector<Command> & CommandBuffer::getDeferredCommands() const
    {
      return m_deferred;
    }

    const std::vector<Command> & CommandBuffer::getForwardCommands() const
    {
      return m_forward;
    }

    const std::vector<Command> & CommandBuffer::getTransparentCommands() const
    {
      return m_transparent;
    }
  }
}