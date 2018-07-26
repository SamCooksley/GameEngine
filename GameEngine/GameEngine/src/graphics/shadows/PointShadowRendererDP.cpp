#include "stdafx.h"

#include "PointShadowRendererDP.h"

namespace engine {
namespace graphics {

  PointShadowRendererDP::PointShadowRendererDP(
    int _size,
    const std::shared_ptr<Shader> & _depth,
    const std::shared_ptr<FilterArray> & _blur
  ) :
    m_depth(_depth),
    m_blur(_blur),
    m_shadowBuffer(FrameBuffer::Create(_size, _size, 2))
  {
    if (!m_depth)
    {
      throw std::invalid_argument("null depth shader");
    }

    m_shadowBuffer->Attach(
      std::make_shared<Texture2DArray>(TextureFormat::DEPTH_COMPONENT32F, _size, _size, 2, 1),
      FrameBufferAttachment::DEPTH
    );
    m_shadowBuffer->setClearColour(glm::vec4(1.0f));
  }

  PointShadowRendererDP::~PointShadowRendererDP()
  { }

  void PointShadowRendererDP::setLight(const glm::vec3 & _position, float _near, float _far)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Attach(CreateShadowMap(), FrameBufferAttachment::COLOUR, 0);    
    m_shadowBuffer->Clear();

    m_lightPosition = _position;
    m_lightSpace = glm::inverse(glm::translate(m_lightPosition));

    m_near = _near;
    m_far = _far;
  }

  void PointShadowRendererDP::Render(const CommandBuffer & _commands)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Clear();

    m_depth->Bind();

    m_depth->setView(m_lightSpace);
    m_depth->setUniform<glm::vec3>("origin", m_lightPosition);
    m_depth->setUniform<float>("near", m_near);
    m_depth->setUniform<float>("far", m_far);

    auto & occluders = _commands.getShadowCasters();
    for (auto & command : occluders)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }

    if (m_blur)
    {
      Texture2DArray & shadowMap = *m_shadowBuffer->getColourAttachment<Texture2DArray>(0);
      m_blur->Apply(shadowMap, shadowMap);
    }
  }

  DualParaboloidShadowMap PointShadowRendererDP::getShadowMap() const
  {
    DualParaboloidShadowMap shadow;
    shadow.lightSpace = m_lightSpace;
    shadow.near = m_near;
    shadow.far = m_far;
    shadow.shadowMap = m_shadowBuffer->getColourAttachment<Texture2DArray>(0);
    return shadow;
  }

  std::shared_ptr<Texture2DArray> PointShadowRendererDP::CreateShadowMap()
  {
    auto texture = std::make_shared<Texture2DArray>(
      TextureFormat::RG32F, m_shadowBuffer->getWidth(), m_shadowBuffer->getHeight(), 2, 1
    );
    texture->setFilter(TextureFilter::LINEAR);
    texture->setWrap(TextureWrap::CLAMP_TO_EDGE);

    return texture;
  }

} } // engine::graphics