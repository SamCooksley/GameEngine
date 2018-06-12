#include "stdafx.h"

#include "ShadowRenderer.h"

namespace engine {
namespace graphics {

  ShadowRenderer::ShadowRenderer(const std::shared_ptr<Shader> & _depth) :
    m_depth(_depth)
  {
    assert(m_depth);
  }

  ShadowRenderer::~ShadowRenderer()
  { }

  void ShadowRenderer::Render(const Camera & _camera, const ShadowCommandBuffer & _occluders)
  {
    m_depth->Bind();
    m_depth->setProjection(_camera.projection);
    m_depth->setView(_camera.view);

    const auto & commands = _occluders.getCommands();
    for (auto & command : commands)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }
  }

} } // engine::graphics