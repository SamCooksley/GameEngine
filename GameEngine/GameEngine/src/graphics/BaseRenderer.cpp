#include "stdafx.h"

#include "BaseRenderer.h"

#include "Resources.h"

namespace engine {
namespace graphics {

  BaseRenderer::BaseRenderer(RenderFlags::Type _flags) :
    Renderer(_flags)
  { 
    m_lights.ambient = glm::vec3(0.1f);
  }
  
  BaseRenderer::~BaseRenderer()
  { }
  
  void BaseRenderer::Add(
    const std::shared_ptr<Mesh> & _mesh,
    const std::shared_ptr<Material> & _material,
    const glm::mat4 & _transform
  )
  {
    Renderer::Add(_mesh, _material, _transform);
  
    m_commands.Add(_mesh, _material, _transform);
  }

  void BaseRenderer::Add(const DirectionalLight & _directional)
  {
    Renderer::Add(_directional);

    m_lights.directional.push_back(_directional);
  }

  void BaseRenderer::Add(const PointLight & _point)
  {
    Renderer::Add(_point);

    m_lights.point.push_back(_point);
  }

  void BaseRenderer::Add(const SpotLight & _spot)
  {
    Renderer::Add(_spot);

    m_lights.spot.push_back(_spot);
  }

  void BaseRenderer::setCamera(const Camera & _camera)
  {
    m_camera = _camera;
  }
  
  void BaseRenderer::End()
  {
    m_commands.Sort(m_camera);
  }
  
  void BaseRenderer::Reset()
  {
    m_commands.Clear();
    m_lights.directional.clear();
    m_lights.point.clear();
    m_lights.spot.clear();
  }
  
  void BaseRenderer::setSkybox(const std::shared_ptr<Skybox> & _skybox)
  {
    m_skybox = _skybox;
  }
  
  void BaseRenderer::setAmbient(const glm::vec3 & _ambient)
  {
    m_lights.ambient = _ambient;
  }

  const CommandBuffer & BaseRenderer::getCommands() const
  {
    return m_commands;
  }

} } // engine::graphics