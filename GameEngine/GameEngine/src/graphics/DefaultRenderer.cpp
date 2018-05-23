#include "stdafx.h"

#include "DefaultRenderer.h"

#include "Graphics.h"
#include "LightBuffer.h"
#include "CameraBuffer.h"

#include "Resources.h"

namespace engine
{
  namespace graphics
  {
    DefaultRenderer::DefaultRenderer() :
      BaseRenderer(RenderFlags::None)
    { 
      auto shader = Resources::Load<Shader>("resources/shaders/gBuffer.shader");
      m_differedMat = Material::Create(shader);

      //TODO: temp
      //CreateGBuffer(1920, 1080);
    }

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
        size_t i = 0;
        for (; i < m_lights.size(); ++i)
        {
          LightBuffer::setLight(buffer, m_lights[i], i);
        }

        for (; i < MAX_LIGHTS; ++i)
        {
          LightBuffer::ClearLight(buffer, i);
        }

        LightBuffer::setAmbient(buffer, m_ambient);
      }

      //m_gBuffer->Bind();
      //m_gBuffer->Clear();

      m_commands.begin()->material.lock()->getShader()->Bind();

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
        shader->Bind();
        material->Bind();

        shader->setModel(command.transform);
        shader->setView(m_camera.view);
        shader->setProjection(m_camera.projection);

        mesh->Render();
        material->Unbind();
      }

     // m_gBuffer->Unbind();

      if (m_skybox)
      {
        m_skybox->Render(m_camera);
      }
    }


    void DefaultRenderer::CreateGBuffer(uint _width, uint _height)
    {
      m_gBuffer = std::make_unique<FrameBuffer>(_width, _height);

      auto position = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
      auto normal = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
      auto colour = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
      auto specular = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
      m_gBuffer->AddRenderBuffer(FrameBufferAttachment::DEPTH_STENCIL, TextureFormat::DEPTH24_STENCIL8);

      if (m_differedMat)
      {
        m_differedMat->setTexture("position", position);
        m_differedMat->setTexture("normal", normal);
        m_differedMat->setTexture("colour", colour);
        m_differedMat->setTexture("specular", specular);
      }
    }
  }
}