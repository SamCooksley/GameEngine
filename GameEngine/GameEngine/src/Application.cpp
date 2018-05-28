#include "stdafx.h"  

#include "Application.h"

#include "graphics\opengl.h"

#include "Input.h"
#include "graphics\Shader.h"

#include "graphics\DefaultRenderer.h"

#include "graphics\Texture2D.h"
#include "graphics\TextureCube.h"

#include "Resources.h"

#include "graphics\CameraBuffer.h"
#include "graphics\LightBuffer.h"

#include "utilities\System.h"

namespace engine
{
  std::unique_ptr<core::Context> Application::s_context;

  void Application::Init(int _argc, char** _argv)
  {
    s_context.reset(new core::Context());

    s_context->state = core::EngineState::Init;

    for (int i = 0; i < _argc; ++i)
    {
      s_context->args.push_back(_argv[i]);
    }

    s_context->glfwContext.reset(new core::glfw());

    s_context->window.reset(new graphics::Window("Engine", 640, 480));
    //s_context->window->setVsync(true);

    s_context->targetFrameTime = 1.f / 60.f;
    s_context->maxUpdatesPerFrame = 5;
    s_context->totalDeltaTime = 0.f;
    s_context->deltaTime = 0.f;

    Input::Init();

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
      throw std::runtime_error("glew error: " + std::to_string(error) + ": " + std::string((const char*)glewGetErrorString(error)));
    }

    debug::Log("OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    debug::Log("GLSL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));
 
    s_context->graphics.glData = std::make_unique<graphics::GLData>();

    s_context->graphics.glData->SetCull(graphics::Cull::BACK);
    s_context->graphics.glData->SetDepth(graphics::Depth::LESS);
    s_context->graphics.glData->EnableBlend(graphics::BlendFactor::SRC_ALPHA, graphics::BlendFactor::ONE_MINUS_SRC_ALPHA);

    //uniform buffers
    {
      auto camera = std::make_unique<graphics::CameraBuffer>();
      s_context->graphics.uniformBuffers.Add(std::move(camera));

      auto lights = std::make_unique<graphics::LightBuffer>();
      s_context->graphics.uniformBuffers.Add(std::move(lights));
    }
    s_context->graphics.errorShader = Resources::Load<graphics::Shader>("resources/shaders/error.shader");


    s_context->graphics.defaultRenderer = std::make_shared<graphics::DefaultRenderer>();
    s_context->graphics.defaultRenderer->setAmbient(glm::vec3(0.1f));

    
    auto defaultShader = Resources::Load<graphics::Shader>("resources/shaders/default.shader");
    s_context->graphics.defaultMaterial = graphics::Material::Create(defaultShader);
   
    auto texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/diffuse.png");
    auto diffuse = texture;
    //texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/diffuse.png");
    s_context->graphics.defaultMaterial->setTexture("diffuse", texture);

    //texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/normal.png");
    texture = Resources::Load<graphics::Texture2D>("resources/textures/flat.png");
    //texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/normal.png");
    s_context->graphics.defaultMaterial->setTexture("normal", texture);

    //texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/roughness.jpg");
    texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/specular.png");
    s_context->graphics.defaultMaterial->setTexture("specular", texture);

    //texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/displacement.png");
    //texture = Resources::Load<graphics::Texture2D>("resources/textures/bricks/displacement.png");
    texture = graphics::Texture2D::Create(64, 64, glm::vec4(0.f));
    s_context->graphics.defaultMaterial->setTexture("displacement", texture);

    texture = graphics::Texture2D::Create(64, 64, glm::vec4(1.f));
    s_context->graphics.defaultMaterial->setTexture("opacity", texture);

    s_context->graphics.defaultMaterial->setUniform("displacementScale", 0.01f);

    s_context->graphics.defaultMaterial->setUniform("shininess", 5.f);
    
    auto skyboxShader = Resources::Load<graphics::Shader>("resources/shaders/skybox.shader");
    auto skyboxMaterial = graphics::Material::Create(skyboxShader);

    std::array<std::string, 6> skyboxTexturePaths;
    for (size_t i = 0; i < 6; ++i)
    {
      skyboxTexturePaths[i] = "resources/textures/skybox/skybox" + std::to_string(i + 1) + ".jpg";
    }

    auto skyboxCube = graphics::TextureCube::Load(skyboxTexturePaths);
    skyboxMaterial->setTexture("cubemap", skyboxCube); 

    //auto inverseCube = Resources::Load<graphics::Mesh>("resources/models/skybox.obj");
    auto skybox = std::make_shared<graphics::Skybox>(skyboxMaterial);
    s_context->graphics.defaultRenderer->setSkybox(skybox);
  }

  void Application::Loop()
  {
    s_context->state = core::EngineState::Running;
    s_context->frameTime.Reset();

    while (s_context->state == core::EngineState::Running)
    {
      Frame();
    }
  }

  void Application::Exit()
  {
    s_context.release();
  }

  void Application::Quit()
  {
    s_context->state = core::EngineState::Exit;
  }

  void Application::Frame()
  {
    if (s_context->targetFrameTime > 0.0f && s_context->deltaTime < s_context->targetFrameTime)
    {
      utilities::Sleep(uint((s_context->targetFrameTime - s_context->deltaTime) * 1000.0f));
    }
    //s_context->totalDeltaTime += s_context->frameTime.getSeconds();
    s_context->frameTime.Reset();

    if (s_context->nextScene)
    {
      ChangeScene();
    }

    s_context->deltaTime = s_context->targetFrameTime;
    
    /*int count = 0;
    while (s_context->totalDeltaTime >= s_context->targetFrameTime)
    {
      s_context->deltaTime = s_context->targetFrameTime;
      HandleEvents();
      Update();
      s_context->totalDeltaTime -= s_context->targetFrameTime;

      if (++count > s_context->maxUpdatesPerFrame) { break; }
    }*/

    HandleEvents();
    Update();
    Render();
  }

  void Application::HandleEvents()
  {
    Input::Reset();
    s_context->glfwContext->PollEvents();
  }

  void Application::Update()
  {
    if (s_context->scene)
    {
      s_context->scene->Update();
    }
  }

  void Application::Render()
  {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    if (s_context->graphics.cameras.empty())
    {
      debug::LogError("Camera Error: no cameras in scene");
    }

    for (auto & cam : s_context->graphics.cameras)
    {
      auto camera = cam.lock();
      if (!camera)
      {
        debug::LogError("Camera Error: destroyed camera in camera list");
        Graphics::RemoveCamera(nullptr);
        continue;
      }

      if (!camera->getEnabled())
      {
        continue;
      }

      camera->SetupRender();

      camera->m_renderer->Start(camera->getCamera());

      if (s_context->scene)
      {
        s_context->scene->Render(*camera->m_renderer);
      }

      camera->m_renderer->End();

      camera->m_renderer->Render();
    }

    s_context->window->Present();
  }

  void Application::ChangeScene()
  {
    s_context->scene = std::move(s_context->nextScene);
    s_context->nextScene = nullptr;

    s_context->scene->Init();
  }
}