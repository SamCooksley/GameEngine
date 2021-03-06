#include "stdafx.h"

#include "DefaultRenderer.h"

#include "Graphics.h"
#include "LightBuffer.h"
#include "CameraBuffer.h"

#include "Lights.h"
#include "Input.h"

namespace engine {
namespace graphics {

  DefaultRenderer::DefaultRenderer() :
    BaseRenderer(RenderFlags::None)
  { 
    m_deferredAmbient = Resources::Load<Shader>("resources/shaders/pbr/deferred/ibl.glsl");
    m_deferredAmbient->setUniform("position", 0);
    m_deferredAmbient->setUniform("normal", 1);
    m_deferredAmbient->setUniform("colour", 2);
    m_deferredAmbient->setUniform("irradianceMap", 3);
    m_deferredAmbient->setUniform("prefilterMap", 4);
    m_deferredAmbient->setUniform("brdfLUT", 5);
    m_deferredAmbient->setUniform("ssao", 6);

    m_deferredDirectional = Resources::Load<Shader>("resources/shaders/pbr/deferred/directional_csm.glsl");
    m_deferredDirectional->setUniform("position", 0);
    m_deferredDirectional->setUniform("normal", 1);
    m_deferredDirectional->setUniform("colour", 2);
    m_deferredDirectional->setUniform("shadowMap", 3);

    m_deferredPoint = Resources::Load<Shader>("resources/shaders/pbr/deferred/point.glsl");
    m_deferredPoint->setUniform("position", 0);
    m_deferredPoint->setUniform("normal", 1);
    m_deferredPoint->setUniform("colour", 2);
    m_deferredPoint->setUniform("shadowMap", 3);

    m_deferredSpot = Resources::Load<Shader>("resources/shaders/pbr/deferred/spot.glsl");
    m_deferredSpot->setUniform("position", 0);
    m_deferredSpot->setUniform("normal", 1);
    m_deferredSpot->setUniform("colour", 2);
    m_deferredSpot->setUniform("shadowMap", 3);

    m_directionalShadowRenderer = std::make_unique<CascadedShadowRenderer>(512, 4);
    m_pointShadowRenderer = std::make_unique<PointShadowRendererCube>(512);
    m_spotShadowRenderer = std::make_unique<SpotShadowRenderer>(512);

    m_hdr = Resources::Load<Shader>("resources/shaders/filters/hdr.glsl");
    m_hdr->setUniform("tex", 0);

    m_environmentFactory = std::make_unique<EnvironmentCaptureFactory>();

    auto brdf = Shader::Load("resources/shaders/pbr/ibl/brdf.glsl");
    brdf->Bind();


    m_brdfLUT = std::make_shared<Texture2D>(TextureFormat::RG32F, 512, 512);
    m_brdfLUT->setWrap(TextureWrap::CLAMP_TO_EDGE);
    m_brdfLUT->setFilter(TextureFilter::LINEAR);

    auto & fb = Graphics::getContext().captureFBO;
    fb->Bind();
    fb->Reset(m_brdfLUT->getWidth(), m_brdfLUT->getHeight());
    fb->Attach(m_brdfLUT, FrameBufferAttachment::COLOUR, 0);
    fb->Clear();

    Graphics::RenderQuad();

    m_ssaoProcess = std::make_unique<SSAO>();
  }
  
  DefaultRenderer::~DefaultRenderer()
  { }
  
  void DefaultRenderer::Render()
  {
    auto target = Graphics::getContext().activeFrameBuffer.lock();

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
  
    GenerateShadows();
    GeneratePerCameraShadows();

    m_target->Bind();

    DeferredRender();
    ForwardRender();    

    target->Bind();

    PostProcess();
  }

  void DefaultRenderer::Resize(int _width, int _height)
  {
    CreateGBuffer(_width, _height);
    CreateTarget(_width, _height);

    m_ssao = std::make_unique<Texture2D>(TextureFormat::R16F, m_position->getWidth(), m_position->getHeight());
  }

  void DefaultRenderer::setSkybox(const std::shared_ptr<Skybox> & _skybox)
  {
    BaseRenderer::setSkybox(_skybox);

    if (m_skybox)
    {
      m_environment = std::make_unique<EnvironmentCapture>();
      *m_environment = m_environmentFactory->Create(*m_skybox->getTexture());
    }
  }
  
  void DefaultRenderer::CreateGBuffer(int _width, int _height)
  {
    if (!m_gBuffer)
    {
      m_gBuffer = FrameBuffer::Create(_width, _height, 1); 
    }
    else 
    { 
      m_gBuffer->Bind(); 
      m_gBuffer->Reset(_width, _height, 1);
    }

    m_position = std::make_shared<Texture2D>(TextureFormat::RGBA32F, _width, _height, 1);
    m_position->setWrap(TextureWrap::CLAMP_TO_EDGE);
    m_position->setFilter(TextureFilter::NEAREST);
    m_gBuffer->Attach(m_position, FrameBufferAttachment::COLOUR, 0);

    m_normal = std::make_shared<Texture2D>(TextureFormat::RGBA32F, _width, _height, 1);
    m_normal->setWrap(TextureWrap::CLAMP_TO_EDGE);
    m_normal->setFilter(TextureFilter::NEAREST);
    m_gBuffer->Attach(m_normal, FrameBufferAttachment::COLOUR, 1);

    m_colour = std::make_shared<Texture2D>(TextureFormat::RGBA32F, _width, _height, 1);
    m_colour->setWrap(TextureWrap::CLAMP_TO_EDGE);
    m_colour->setFilter(TextureFilter::NEAREST);
    m_gBuffer->Attach(m_colour, FrameBufferAttachment::COLOUR, 2);

    m_gBuffer->Attach(
      std::make_shared<Texture2D>(TextureFormat::DEPTH_COMPONENT32, _width, _height, 1), 
      FrameBufferAttachment::DEPTH
    );
  }

  void DefaultRenderer::CreateTarget(int _width, int _height)
  {
    if (!m_target)
    {
      m_target = FrameBuffer::Create(_width, _height, 1);
    }
    else
    {
      m_target->Bind();
      m_target->Reset(_width, _height, 1);
    }
    
    auto tex = std::make_shared<Texture2D>(TextureFormat::RGBA32F, _width, _height, 1);
    m_target->Attach(tex, FrameBufferAttachment::COLOUR, 0);

    tex = std::make_shared<Texture2D>(TextureFormat::DEPTH_COMPONENT32, _width, _height, 1);
    m_target->Attach(tex, FrameBufferAttachment::DEPTH);
  }

  void DefaultRenderer::GenerateShadows()
  {
    if (m_pointShadowRenderer)
    {
      for (auto & point : m_lights.point)
      {
        if (!point.shadowData.enable) { continue; }

        m_pointShadowRenderer->setLight(
          point.position,
          point.shadowData.near, point.shadowData.far
        );

        m_pointShadowRenderer->Render(m_commands);

        point.shadow = m_pointShadowRenderer->getShadowMap();
      }
    }

    if (m_spotShadowRenderer)
    {
      for (auto & spot : m_lights.spot)
      {
        if (!spot.shadowData.enable) { continue; }

        m_spotShadowRenderer->setLight(
          spot.position, spot.direction,
          spot.fov,
          spot.shadowData.near, spot.shadowData.far
        );

        m_spotShadowRenderer->Render(m_commands);

        spot.shadow = m_spotShadowRenderer->getShadowMap();
      }
    }
  }

  void DefaultRenderer::GeneratePerCameraShadows()
  {
    if (m_directionalShadowRenderer)
    {
      for (auto & dir : m_lights.directional)
      {
        if (!dir.shadowData.enable) { continue; }

        m_directionalShadowRenderer->setLight(dir.direction, m_camera);

        m_directionalShadowRenderer->Render(m_commands);

        dir.shadow = m_directionalShadowRenderer->getShadowMap();
      }
    }
  }

  void DefaultRenderer::DeferredRender()
  {
    std::shared_ptr<Material> prevMat;
    auto & deferredCommands = m_commands.getDeferredCommands();

    if (deferredCommands.empty()) { return; }
    
    auto target = Graphics::getContext().activeFrameBuffer.lock();

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

    if (m_ssaoProcess)
    {
      m_ssaoProcess->GenerateAO(*m_ssao, m_camera, *m_position, *m_normal);
    }

    target->Bind();

    m_position->Bind(0);
    m_normal->Bind(1);
    m_colour->Bind(2);

    m_deferredAmbient->Bind();
    m_environment->irradiance->Bind(3);
    m_environment->prefiltered->Bind(4);
    m_brdfLUT->Bind(5);
    if (m_ssao && !Input::getKey(KeyCode::Q))
    {
      m_deferredAmbient->setUniform<int>("hasAO", 1);
      m_ssao->Bind(6);
    }
    else
    {
      m_deferredAmbient->setUniform<int>("hasAO", 0);
    }

    Graphics::RenderQuad();

    if (!m_lights.directional.empty())
    {
      m_deferredDirectional->Bind();

      for (auto & dir : m_lights.directional)
      {
        m_deferredDirectional->setUniform("light.colour", dir.colour);
        m_deferredDirectional->setUniform("light.direction", dir.direction);

        if (dir.shadow.shadowMap)
        {
          m_deferredDirectional->setUniform("shadow", 1);

          int size = glm::min(dir.shadow.distances.size(), MAX_DIRECTIONAL_CASCADES);

          m_deferredDirectional->setUniform<int>("cascadeCount", size);

          dir.shadow.shadowMap->Bind(3);
          for (int i = 0; i < size; ++i)
          {
            std::string arr = '[' + std::to_string(i) + ']';
            m_deferredDirectional->setUniform("lightSpace" + arr, dir.shadow.lightSpaces[i]);
            m_deferredDirectional->setUniform("distance" + arr, dir.shadow.distances[i]);
          }
        }
        else
        {
          m_deferredDirectional->setUniform("shadow", 0);
        }

        Graphics::RenderQuad();
      }
    }

    if (!m_lights.point.empty())
    {
      m_deferredPoint->Bind();

      for (auto & point : m_lights.point)
      {
        m_deferredPoint->setUniform("light.colour", point.colour);
        m_deferredPoint->setUniform("light.position", point.position);
        m_deferredPoint->setUniform("light.radius", point.radius);

        if (point.shadow.shadowMap)
        {
          m_deferredPoint->setUniform<int>("shadow", 1);
          m_deferredPoint->setUniform<float>("near", point.shadow.near);
          m_deferredPoint->setUniform<float>("far", point.shadow.far);
          point.shadow.shadowMap->Bind(3);
        }
        else
        {
          m_deferredPoint->setUniform<int>("shadow", 0);
        }

        Graphics::RenderQuad();
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
        m_deferredSpot->setUniform("light.radius", spot.radius);

        if (spot.shadow.shadowMap)
        {
          m_deferredSpot->setUniform<int>("shadow", 1);
          m_deferredSpot->setUniform<glm::mat4>("lightSpace", spot.shadow.lightSpace);
          spot.shadow.shadowMap->Bind(3);
        }
        else
        {
          m_deferredSpot->setUniform<int>("shadow", 0);
        }

        Graphics::RenderQuad();
      }
    }

    m_gBuffer->Bind(FrameBufferBind::READ);
    target->Bind(FrameBufferBind::WRITE);
    FrameBuffer::Blit(0, 0, m_gBuffer->getWidth(), m_gBuffer->getHeight(), 0, 0, target->getWidth(), target->getHeight(), BufferBit::DEPTH, TextureFilter::NEAREST);
  }

  void DefaultRenderer::ForwardRender()
  {
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

  void DefaultRenderer::PostProcess()
  {
    m_hdr->Bind();
    m_target->getColourAttachment<Texture2D>(0)->Bind(0);

    Graphics::RenderQuad();
  }

} } // engine::graphics