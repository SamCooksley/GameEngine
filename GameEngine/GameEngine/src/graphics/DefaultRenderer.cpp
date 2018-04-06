#include "stdafx.h"

#include "DefaultRenderer.h"

#include "Graphics.h"
#include "LightBuffer.h"
#include "CameraBuffer.h"

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
      if (Graphics::HasUniformBuffer(CameraBuffer::name))
      {
        auto & buffer = Graphics::getUniformBuffer(CameraBuffer::name);
        CameraBuffer::setCamera(buffer, m_camera);
      }

      if (Graphics::HasUniformBuffer(LightBuffer::name))
      {
        auto & buffer = Graphics::getUniformBuffer(LightBuffer::name);
        for (size_t i = 0; i < m_lights.size(); ++i)
        {
          LightBuffer::setLight(buffer, m_lights[i], i);
        }

        LightBuffer::setAmbient(buffer, m_ambient);
      }

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

      if (m_skybox)
      {
        m_skybox->Render(m_camera);
      }
    }
  }
}