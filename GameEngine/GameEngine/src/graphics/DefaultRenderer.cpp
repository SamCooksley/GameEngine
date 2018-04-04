#include "stdafx.h"

#include "DefaultRenderer.h"

namespace engine
{
  namespace graphics
  {
    DefaultRenderer::DefaultRenderer() :
      BaseRenderer(RenderFlags::None)
    { }

    DefaultRenderer::~DefaultRenderer()
    { }

    void DefaultRenderer::Render()
    {
      for (auto & command : m_commands)
      {
        auto mesh = command.mesh.lock();
        auto material = command.material.lock();

        if (!mesh || !material)
        {
          debug::LogError(
            "DefaultRenderer Error: mesh or material missing from command. Has it been destroyed?"
          );
          continue;
        }

        auto shader = material->getShader();

        material->Bind();

        for (size_t i = 0; i < MAX_LIGHTS; ++i)
        {
          if (i < m_lights.size())
          {
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].type"), static_cast<int>(m_lights[i].type));
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].linear"), m_lights[i].linear);
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].quadratic"), m_lights[i].quadratic);
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].cutoff"), m_lights[i].cutoff);
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].outerCutoff"), m_lights[i].outerCutoff);
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].position"), m_lights[i].position);
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].direction"), m_lights[i].direction);
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].colour"), m_lights[i].colour);
          }
          else
          {
            shader->setUniform(shader->getUniformLocation("lights[" + std::to_string(i) + "].type"), -1);
          }
        }

        shader->setUniform(shader->getUniformLocation("view_position_world"), m_camera.position);

        shader->setModel(command.transform);
        shader->setView(m_camera.view);
        shader->setProjection(m_camera.projection);

        mesh->Render(*shader);
        material->Unbind();
      }

      if (m_skybox)
      {
        m_skybox->Render(m_camera);
      }
    }
  }
}