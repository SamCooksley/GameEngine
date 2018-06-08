#include "stdafx.h"

#include "ShadowMapping.h"

namespace engine { 
namespace graphics {

  ShadowMapping::ShadowMapping(uint _width, uint _height, TextureFormat _format)
  {
    m_renderer = std::make_unique<ShadowRenderer>();

    m_frameBuffer = FrameBuffer::Create(_width, _height);
    m_shadowMap = m_frameBuffer->AddTexture(FrameBufferAttachment::DEPTH, _format, TextureDataType::FLOAT);
    glm::vec4 borderColor(1.f);
    GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor)));
    m_shadowMap->setWrap(TextureWrap::CLAMP_TO_BORDER);
  }

  ShadowMapping::~ShadowMapping()
  { }

  const glm::mat4 & ShadowMapping::getLightSpace() const
  {
    return m_lightSpace;
  }

  const std::shared_ptr<Texture2D> & ShadowMapping::getShadowMap() const
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