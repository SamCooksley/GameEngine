#include "stdafx.h"

#include "ShadowRenderer.h"

namespace engine {
namespace graphics {

  ShadowRenderer::ShadowRenderer(const std::shared_ptr<Shader> & _depth) :
    m_depth(_depth)
  {
    if (!m_depth)
    {
      throw std::runtime_error("no shader in shadow renderer");
    }
  }

  ShadowRenderer::~ShadowRenderer()
  { }

  void ShadowRenderer::setDirectional(const glm::vec3 & _direction)
  {
    throw not_implemented("ShadowRenderer.setDirectional(const glm::vec3 &) not implemented");
  }

  void ShadowRenderer::setDirectional(const glm::vec3 & _direction, const Camera & _target)
  {
    throw not_implemented("ShadowRenderer.setDirectional(const glm::vec3 &, const Camera &) not implemented");
  }

  void ShadowRenderer::setPoint(const glm::vec3 & _position, float _radius)
  {
    throw not_implemented("ShadowRenderer.setPoint(const glm::vec3 &, float) not implemented");
  }

  void ShadowRenderer::setSpot(const glm::vec3 & _position, const glm::vec3 & _direction, float _fov, float _distance)
  {
    throw not_implemented("ShadowRenderer.setSpot(const glm::vec3 &, const glm::vec3, float, float) not implemented");
  }

  void ShadowRenderer::RenderCommands(const ShadowCommandBuffer & _occluders)
  {
    auto commands = _occluders.getCommands();
    for (auto & command : commands)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }
  }

} } // engine::graphics