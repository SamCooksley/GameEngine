#include "stdafx.h"

#include "DefaultRenderer.h"

#include "Graphics.h"
#include "LightBuffer.h"
#include "CameraBuffer.h"

namespace engine {
namespace graphics {

  DefaultRenderer::DefaultRenderer(const std::shared_ptr<Material> & _deferred) :
    BaseRenderer(RenderFlags::None),
    m_deferredMat(_deferred)
  { }
  
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
      size_t i = 0u;
  
      size_t size = m_lights.size();
      if (size > LightBuffer::max_lights)
      {
        debug::LogWarning("Maximum amount of lights reached.");
  
        size = LightBuffer::max_lights;
      }
  
      lightBuffer->Bind();
  
      for (; i < size; ++i)
      {
        lightBuffer->setLight(m_lights[i], i);
      }
  
      for (; i < LightBuffer::max_lights; ++i)
      {
        lightBuffer->ClearLight(i);
      }
  
      lightBuffer->setAmbient(m_ambient);
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

      m_deferredMat->Bind();

      fbTarget->RenderToNDC();

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
  
    auto position = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
    auto normal = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
    auto colour = m_gBuffer->AddTexture(FrameBufferAttachment::COLOUR, TextureFormat::RGBA16F, TextureDataType::FLOAT);
    m_gBuffer->AddRenderBuffer(FrameBufferAttachment::DEPTH, TextureFormat::DEPTH_COMPONENT24);
  
    if (m_deferredMat)
    {
      m_deferredMat->setTexture("position", position);
      m_deferredMat->setTexture("normal", normal);
      m_deferredMat->setTexture("colour", colour);
    }
  }

} } // engine::graphics