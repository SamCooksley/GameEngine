#include "stdafx.h"

#include "ShadowMap.h"

#include "Graphics.h"

namespace engine { 
namespace graphics {

  /*
  ShadowMap::ShadowMap(
    uint _width, uint _height, 
    TextureFormat _format,
    const std::shared_ptr<Shader> & _depth
  )
  {
    m_depth = _depth ? _depth : Graphics::getContext().depthShader;

    m_frameBuffer = FrameBuffer::Create(_width, _height);
    m_shadowMap = m_frameBuffer->AddShadow2D(_format);
    m_frameBuffer->Clear();
  }

  ShadowMap::~ShadowMap()
  { }

  const glm::mat4 & ShadowMap::getLightSpace() const
  {
    return m_lightSpace;
  }

  const std::shared_ptr<Shadow2D> & ShadowMap::getShadowMap() const
  {
    return m_shadowMap;
  }


  void ShadowMap::GenerateShadowMap(const Camera & _camera, const ShadowCommandBuffer & _occluders)
  {
    m_frameBuffer->Bind();
    m_frameBuffer->Clear();

    m_lightSpace = _camera.vp;

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
  */

} } // engine::graphics 