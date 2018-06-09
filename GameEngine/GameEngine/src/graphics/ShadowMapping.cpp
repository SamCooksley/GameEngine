#include "stdafx.h"

#include "ShadowMapping.h"

namespace engine { 
namespace graphics {

  ShadowMapping::ShadowMapping(uint _width, uint _height, TextureFormat _format)
  {
    m_renderer = std::make_unique<ShadowRenderer>();

    m_frameBuffer = FrameBuffer::Create(_width, _height);
    m_shadowMap = m_frameBuffer->AddShadow2D(_format);
  }

  ShadowMapping::~ShadowMapping()
  { }

  const glm::mat4 & ShadowMapping::getLightSpace() const
  {
    return m_lightSpace;
  }

  const std::shared_ptr<Shadow2D> & ShadowMapping::getShadowMap() const
  {
    return m_shadowMap;
  }
  
  ShadowRenderer * ShadowMapping::getRenderer()
  {
    return m_renderer.get();
  }

  void ShadowMapping::Setup(const Camera & _camera)
  {
    m_frameBuffer->Bind();
    m_frameBuffer->Clear();

    m_lightSpace = _camera.vp;
    m_renderer->Start(_camera);
  }

} } // engine::graphics 