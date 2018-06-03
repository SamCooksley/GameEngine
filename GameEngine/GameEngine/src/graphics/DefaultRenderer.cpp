#include "stdafx.h"

#include "DefaultRenderer.h"

#include "Graphics.h"
#include "LightBuffer.h"
#include "CameraBuffer.h"

#include "Resources.h"

namespace engine {
namespace graphics {

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

} } // engine::graphics