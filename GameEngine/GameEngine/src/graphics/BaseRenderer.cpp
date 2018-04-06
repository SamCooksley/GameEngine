#include "stdafx.h"

#include "BaseRenderer.h"

namespace engine
{
  namespace graphics
  {
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
      std::weak_ptr<Mesh> _mesh,
      std::weak_ptr<Material> _material,
      const glm::mat4 & _transform
    )
    {
      Renderer::Add(_mesh, _material, _transform);

      m_commands.push_back({
        std::move(_mesh),
        std::move(_material),
        _transform
      });
    }

    void BaseRenderer::Add(const Light & _light)
    {
      Renderer::Add(_light);

      m_lights.push_back(_light);
    }

    void BaseRenderer::End()
    { }

    void BaseRenderer::Reset()
    {
      m_commands.clear();
      m_lights.clear();
    }

    void BaseRenderer::setSkybox(std::shared_ptr<Skybox> _skybox)
    {
      m_skybox = std::move(_skybox);
    }

    void BaseRenderer::setAmbient(const glm::vec3 & _ambient)
    {
      m_ambient = _ambient;
      
    }
  }
}