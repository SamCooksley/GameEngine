#include "stdafx.h"

#include "DefaultRenderer.h"

#include "debug\Debug.h"

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

        shader->setModel(command.transform);
        shader->setView(m_camera.view);
        shader->setProjection(m_camera.projection);

        mesh->Render(*shader);
        material->Unbind();
      }
    }
  }
}