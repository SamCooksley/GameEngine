#include "stdafx.h"

#include "Skybox.h"

namespace engine
{
  namespace graphics
  {
    Skybox::Skybox(
      std::shared_ptr<Material> _material,
      std::shared_ptr<Mesh> _mesh
    ) :
      m_material(std::move(_material)),
      m_mesh(std::move(_mesh))
    { }

    Skybox::~Skybox()
    { }

    void Skybox::Render(Camera & _camera)
    {
      auto shader = m_material->getShader();
      shader->Bind();
      m_material->Bind();


      shader->setView(_camera.view);
      shader->setProjection(_camera.projection);

      GLCALL(glDepthFunc(GL_LEQUAL));
      m_mesh->Render();
      GLCALL(glDepthFunc(GL_LESS));

      m_material->Unbind();
    }

    void Skybox::Render()
    {
      m_material->Bind();

      GLCALL(glDepthFunc(GL_LEQUAL));
      m_mesh->Render();
      GLCALL(glDepthFunc(GL_LESS));

      m_material->Unbind();
    }
  }
}