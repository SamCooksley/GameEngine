#include "stdafx.h"

#include "DefaultRenderer.h"

#include "Graphics.h"
#include "LightBuffer.h"
#include "CameraBuffer.h"

#include "Lights.h"

namespace engine {
namespace graphics {

  DefaultRenderer::DefaultRenderer() :
    BaseRenderer(RenderFlags::None)
  { 
    m_deferredAmbient = Resources::Load<Shader>("resources/shaders/deferred/ambient.shader");
    m_deferredAmbient->setUniform("colour", 2);

    m_deferredDirectional = Resources::Load<Shader>("resources/shaders/deferred/directional_csm.shader");
    m_deferredDirectional->setUniform("position", 0);
    m_deferredDirectional->setUniform("normal", 1);
    m_deferredDirectional->setUniform("colour", 2);
    m_deferredDirectional->setUniform("shadowMap", 3);

    m_deferredPoint = Resources::Load<Shader>("resources/shaders/deferred/point.shader");
    m_deferredPoint->setUniform("position", 0);
    m_deferredPoint->setUniform("normal", 1);
    m_deferredPoint->setUniform("colour", 2);

    m_deferredSpot = Resources::Load<Shader>("resources/shaders/deferred/spot.shader");
    m_deferredSpot->setUniform("position", 0);
    m_deferredSpot->setUniform("normal", 1);
    m_deferredSpot->setUniform("colour", 2);
  }
  
  DefaultRenderer::~DefaultRenderer()
  { }
  
  void DefaultRenderer::Render()
  {
    auto cameraBuffer = Graphics::getUniformBuffer<CameraBuffer>();
    if (cameraBuffer != nullptr)
    {
      cameraBuffer->Bind();
      cameraBuffer->setCamera(m_camera);
    }
  
    auto lightBuffer = Graphics::getUniformBuffer<LightBuffer>();
    if (lightBuffer != nullptr)
    {
      lightBuffer->Bind();
      lightBuffer->setLights(m_lights);
    }
  
    std::shared_ptr<Material> prevMat;
    auto & deferredCommands = m_commands.getDeferredCommands();

    if (!deferredCommands.empty())
    {
      auto fbTarget = Graphics::getContext().activeFrameBuffer.lock();

      m_gBuffer->Bind();
      m_gBuffer->Clear();

      for (auto & command : deferredCommands)
      {
        if (command.material != prevMat)
        {
          command.material->Bind();
          prevMat = command.material;
        }

        const auto & shader = command.material->getShader();
        shader->setModel(command.transform);
        shader->setView(m_camera.view);
        shader->setProjection(m_camera.projection);

        command.mesh->Render();
      }

      fbTarget->Bind();

      m_position->Bind(0);
      m_normal->Bind(1);
      m_colour->Bind(2);

      m_deferredAmbient->Bind();
      m_deferredAmbient->setUniform("light", m_lights.ambient);

      fbTarget->RenderToNDC();

      if (!m_lights.directional.empty())
      {
        m_deferredDirectional->Bind();

        for (auto & dir : m_lights.directional)
        {
          m_deferredDirectional->setUniform("light.colour", dir.colour);
          m_deferredDirectional->setUniform("light.direction", dir.direction);

          if (dir.shadow)
          {
            m_deferredDirectional->setUniform("shadow", 1);

            int size = glm::min(dir.shadow->distance.size(), MAX_DIRECTIONAL_CASCADES);

            dir.shadow->shadowMap->Bind(3);
            for (int i = 0; i < size; ++i)
            {
              std::string arr = '[' + std::to_string(i) + ']';
              m_deferredDirectional->setUniform("lightSpace" + arr, dir.shadow->lightSpace[i]);
              m_deferredDirectional->setUniform("distance" + arr, dir.shadow->distance[i]);
            }
          }
          else
          {
            m_deferredDirectional->setUniform("shadow", 0);
          }

          fbTarget->RenderToNDC();
        }
      }
      
      if (!m_lights.point.empty())
      {
        m_deferredPoint->Bind();

        for (auto & point : m_lights.point)
        {
          m_deferredPoint->setUniform("light.colour", point.colour);
          m_deferredPoint->setUniform("light.position", point.position);
          m_deferredPoint->setUniform("light.atten.linear", point.atten.linear);
          m_deferredPoint->setUniform("light.atten.quadratic", point.atten.quadratic);

          fbTarget->RenderToNDC();
        }
      }

      if (!m_lights.spot.empty())
      {
        m_deferredSpot->Bind();

        for (auto & spot : m_lights.spot)
        {
          m_deferredSpot->setUniform("light.colour", spot.colour);
          m_deferredSpot->setUniform("light.position", spot.position);
          m_deferredSpot->setUniform("light.direction", spot.direction);
          m_deferredSpot->setUniform("light.cutoff", spot.cutoff);
          m_deferredSpot->setUniform("light.outerCutoff", spot.outerCutoff);
          m_deferredSpot->setUniform("light.atten.linear", spot.atten.linear);
          m_deferredSpot->setUniform("light.atten.quadratic", spot.atten.quadratic);

          fbTarget->RenderToNDC();
        }
      }
      
      m_gBuffer->Bind(FrameBufferBind::READ);
      fbTarget->Bind(FrameBufferBind::WRITE);
      m_gBuffer->Blit(*fbTarget, BufferBit::DEPTH);

      fbTarget->Bind();
    }

    prevMat = nullptr;
    auto & forwardCommands = m_commands.getForwardCommands();
  
    for (auto & command : forwardCommands)
    {
      if (command.material != prevMat)
      {
        command.material->Bind();
        prevMat = command.material;
      }
  
      const auto & shader = command.material->getShader();
      shader->setModel(command.transform);
      shader->setView(m_camera.view);
      shader->setProjection(m_camera.projection);
  
      command.mesh->Render();
    }
  
    if (m_skybox)
    {
      m_skybox->Render(m_camera);
    }
  
    prevMat = nullptr;
    auto & transparentCommands = m_commands.getTransparentCommands();
  
    for (auto & command : transparentCommands)
    {
      if (command.material != prevMat)
      {
        command.material->Bind();
        prevMat = command.material;
      }
  
      const auto & shader = command.material->getShader();
      shader->setModel(command.transform);
      shader->setView(m_camera.view);
      shader->setProjection(m_camera.projection);
  
      command.mesh->Render();
    }     

    /*
    auto tmp = Shader::Load("resources/shaders/debug_shadow.shader");

    tmp->setUniform("tex", 0);
    tmp->setUniform("depth", 0);
    m_lights.directional[0].shadow->shadowMap->Bind(0);
    m_gBuffer->RenderToNDC();  
    */
    
  }

  void DefaultRenderer::Resize(uint _width, uint _height)
  {
    if (!m_gBuffer)
    {
      CreateGBuffer(_width, _height);
    }
    else
    {
      m_gBuffer->Resize(_width, _height);
    }
  }
  
  void DefaultRenderer::CreateGBuffer(uint _width, uint _height)
  {
    m_gBuffer = FrameBuffer::Create(_width, _height);
  
    m_position = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA32F);
    m_normal = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA32F);
    m_colour = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA32F);
    m_gBuffer->AddRenderBuffer(FrameBufferAttachment::DEPTH, TextureFormat::DEPTH_COMPONENT32);
  }

} } // engine::graphics