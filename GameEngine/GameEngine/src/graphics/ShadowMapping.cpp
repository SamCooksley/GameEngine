#include "stdafx.h"

#include "ShadowMapping.h"

namespace engine { 
namespace graphics {

  ShadowMapping::ShadowMapping(uint _width, uint _height, TextureFormat _format)
  {
    m_renderer = std::make_unique<ShadowRenderer>();

    m_frameBuffer = FrameBuffer::Create(_width, _height);
    m_shadowMap = m_frameBuffer->AddTexture(FrameBufferAttachment::DEPTH, _format, TextureDataType::FLOAT);
  }

  ShadowMapping::~ShadowMapping()
  { }

  const std::shared_ptr<Texture2D> & ShadowMapping::getShadowMap() const
  {
    return m_shadowMap;
  }

  void ShadowMapping::Setup()
  {
    m_frameBuffer->Bind();
  }

} } // engine::graphics 