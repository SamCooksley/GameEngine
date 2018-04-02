#include "stdafx.h"

#include "MeshRenderer.h"

#include "GameObject.h"
#include "debug\Debug.h"
#include "graphics\Graphics.h"

namespace engine
{
  MeshRenderer::MeshRenderer()
  { }

  MeshRenderer::~MeshRenderer()
  { }

  void MeshRenderer::OnAwake()
  {
    Component::OnAwake();

    if (!m_material)
    {
      m_material = Graphics::getDefaultMaterial();
    }
  }

  void MeshRenderer::OnRender(graphics::Renderer & _renderer)
  {
    Component::OnRender(_renderer);

    auto transform = getGameObject()->getComponent<Transform>();
    if (!transform)
    {
      debug::LogError(
        "MeshRenderer Error: " + getGameObject()->getName() + " does not have a transform component."
      );
      return;
    }

    if (!m_mesh || !m_material)
    {
      return;
    }

    glm::mat4 model;
    transform->get(nullptr, nullptr, nullptr, &model);

    _renderer.Add(m_mesh, m_material, model);
  }

  void MeshRenderer::setMesh(std::shared_ptr<graphics::Mesh> _mesh)
  {
    m_mesh = std::move(_mesh);
  }

  void MeshRenderer::setMaterial(std::shared_ptr<graphics::Material> _material)
  {
    m_material = std::move(_material);
  }
}