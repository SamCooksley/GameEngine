#include "stdafx.h"

#include "SpotShadowRenderer.h"
namespace engine {
namespace graphics {

  SpotShadowRenderer::SpotShadowRenderer(
    int _size,
    const std::shared_ptr<Shader> & _depth,
    const std::shared_ptr<Filter> & _blur
  ) :
    m_depth(_depth),
    m_blur(_blur),
    m_shadowBuffer(FrameBuffer::Create(_size, _size, 1))
  {
    m_shadowBuffer->AttachDepth(
      std::make_shared<Shadow2D>(TextureFormat::DEPTH_COMPONENT32F, _size, _size)
    );

    m_shadowBuffer->setClearColour(glm::vec4(1.f));
  }

  SpotShadowRenderer::~SpotShadowRenderer()
  { }

  void SpotShadowRenderer::setLight(
    const glm::vec3 & _position, const glm::vec3 & _direction,
    float _fov, 
    float _near, float _far
  )
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Attach(CreateShadowMap(), FrameBufferAttachment::COLOUR, 0);
    m_shadowBuffer->Clear();

    m_camera = Camera(CameraType::PERSPECTIVE,
      _fov, 1.f, _near, _far,
      glm::lookAt(_position, _position - _direction, glm::vec3(0.f, 1.f, 0.f)),
      _position
    );
  }

  void SpotShadowRenderer::Render(const CommandBuffer & _commands)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Clear();

    m_depth->Bind();
    m_depth->setUniform("vp", m_camera.vp);

    auto & occluders = _commands.getShadowCasters();
    for (auto & command : occluders)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }

    if (m_blur)
    {
      Texture2D & shadow = *m_shadowBuffer->getColourAttachment<Texture2D>(0);
      m_blur->Apply(shadow, shadow);
    }
  }

  SpotShadowMap SpotShadowRenderer::getShadowMap() const
  {
    SpotShadowMap shadow;
    shadow.lightSpace = m_camera.vp;
    shadow.shadowMap = m_shadowBuffer->getColourAttachment<Texture2D>(0);
    return shadow;
  }

  std::shared_ptr<Texture2D> SpotShadowRenderer::CreateShadowMap()
  {
    auto shadow = std::make_shared<Texture2D>(
      TextureFormat::RG32F, m_shadowBuffer->getWidth(), m_shadowBuffer->getHeight(), 1
    );

    shadow->setFilter(TextureFilter::LINEAR);

    shadow->setBorder(glm::vec4(1.f));
    shadow->setWrap(TextureWrap::CLAMP_TO_BORDER);

    return shadow;
  }

} } // engine::graphics