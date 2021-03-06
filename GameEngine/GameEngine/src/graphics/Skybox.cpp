#include "stdafx.h"

#include "Skybox.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  Skybox::Skybox(
    const std::shared_ptr<Material> & _material,
    const std::shared_ptr<Mesh> & _mesh
  ) :
    m_material(_material),
    m_mesh(_mesh)
  { }
  
  Skybox::~Skybox()
  { }
  
  void Skybox::Render(const Camera & _camera)
  {
    auto shader = m_material->getShader();
    m_material->Bind();
  
    shader->setView(_camera.view);
    shader->setProjection(_camera.projection);
  
    m_mesh->Render();
  }

  std::shared_ptr<TextureCube> Skybox::getTexture() const
  {
    std::shared_ptr<TextureCube> tex;

    m_material->getTexture("cubemap", &tex);

    return tex;
  }

  void Skybox::setTexture(const std::shared_ptr<TextureCube> & _texture)
  {
    m_material->setTexture("cubemap", _texture);
  }

} } // engine::graphics