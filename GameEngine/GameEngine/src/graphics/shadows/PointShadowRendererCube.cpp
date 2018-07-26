#include "stdafx.h"

#include "PointShadowRendererCube.h"

namespace engine {
namespace graphics {

  PointShadowRendererCube::PointShadowRendererCube(int _size, const std::shared_ptr<Shader> & _depth) :
    m_camera(), 
    m_depth(_depth),
    m_shadowBuffer(FrameBuffer::Create(_size, _size, 6))
  {
    m_shadowBuffer->Attach(
      std::make_shared<TextureCube>(TextureFormat::DEPTH_COMPONENT32F, m_shadowBuffer->getWidth(), m_shadowBuffer->getHeight(), 1),
      FrameBufferAttachment::DEPTH
    );

    m_shadowBuffer->setClearColour(glm::vec4(1.f));
  }

  PointShadowRendererCube::~PointShadowRendererCube()
  { }

  void PointShadowRendererCube::setLight(const glm::vec3 & _position, float _near, float _far)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Attach(CreateShadowMap(), FrameBufferAttachment::COLOUR, 0);
    m_shadowBuffer->Clear();

    m_camera = CameraCube(_position, _near, _far);
  }

  void PointShadowRendererCube::Render(const CommandBuffer & _commands)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Clear();

    m_depth->Bind();

    for (size_t i = 0; i < 6; ++i)
    {
      m_depth->setUniform<glm::mat4>("vp[" + std::to_string(i) + ']', m_camera.getCamera(i).vp);
    }
    m_depth->setUniform<glm::vec3>("position", m_camera.getPosition());

    m_depth->setUniform<float>("near", m_camera.getNear());
    m_depth->setUniform<float>("far", m_camera.getFar());

    auto & occluders = _commands.getShadowCasters();
    for (auto & command : occluders)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }
  }

  CubeShadowMap PointShadowRendererCube::getShadowMap() const
  {
    CubeShadowMap shadow;
    shadow.near = m_camera.getNear();
    shadow.far = m_camera.getFar();
    shadow.shadowMap = m_shadowBuffer->getColourAttachment<TextureCube>(0);
    return shadow;
  }

  std::shared_ptr<TextureCube> PointShadowRendererCube::CreateShadowMap()
  {
    auto shadow = std::make_shared<TextureCube>(
      TextureFormat::RG32F, m_shadowBuffer->getWidth(), m_shadowBuffer->getHeight(), 1
    );

    shadow->setFilter(TextureFilter::LINEAR);

    return shadow;
  }

} } // engine::graphics