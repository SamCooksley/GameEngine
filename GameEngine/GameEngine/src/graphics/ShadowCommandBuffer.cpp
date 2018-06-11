#include "stdafx.h"

#include "ShadowCommandBuffer.h"

namespace engine {
namespace graphics {

  void ShadowCommandBuffer::Add(
    const std::shared_ptr<Mesh> & _mesh,
    const glm::mat4 & _transform
  )
  {
    assert(_mesh);

    m_commands.push_back({ _mesh, _transform });
  }

  void ShadowCommandBuffer::Clear()
  {
    m_commands.clear();
  }

  const std::vector<ShadowCommand> & ShadowCommandBuffer::getCommands() const
  {
    return m_commands;
  }

} } // engine::graphics