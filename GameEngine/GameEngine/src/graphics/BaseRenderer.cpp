#include "stdafx.h"

#include "BaseRenderer.h"

namespace engine {
namespace graphics {

  BaseRenderer::BaseRenderer(RenderFlags::Type _flags) :
    Renderer(_flags)
  { }
  
  BaseRenderer::~BaseRenderer()
  { }
  
  void BaseRenderer::Start(const Camera & _camera)
  {
    Reset();
  
    m_camera = _camera;
  }
  
  void BaseRenderer::Add(
    const std::shared_ptr<Mesh> & _mesh,
    const std::shared_ptr<Material> & _material,
    const glm::mat4 & _transform
  )
  {
    Renderer::Add(_mesh, _material, _transform);
  
    m_commands.Add(_mesh, _material, _transform);
  }
  
  void BaseRenderer::Add(const Light & _light)
  {
    Renderer::Add(_light);
  
    m_lights.push_back(_light);
  }
  
  void BaseRenderer::End()
  {
    m_commands.Sort(m_camera);
  }
  
  void BaseRenderer::Reset()
  {
    m_commands.Clear();
    m_lights.clear();
  }
  
  void BaseRenderer::setSkybox(const std::shared_ptr<Skybox> & _skybox)
  {
    m_skybox = _skybox;
  }
  
  void BaseRenderer::setAmbient(const glm::vec3 & _ambient)
  {
    m_ambient = _ambient;
  }

} } // engine::graphics