#include "stdafx.h"

#include "CSMRenderer.h"

namespace engine {
namespace graphics {

  void CSMRenderer::setDirectional(const glm::vec3 & _direction, const Camera & _target)
  {

  }

  void CSMRenderer::Render(const ShadowCommandBuffer & _occluders)
  {
    m_depth->Bind();
    for (int i = 0; i < 4; ++i)
    {
      m_depth->setUniform("vp[" + std::to_string(i) + ']', m_camera[i].vp);
    }

    RenderCommands(_occluders);
  }


} } // engine::graphics