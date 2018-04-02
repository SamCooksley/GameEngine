#include "stdafx.h"  

#include "Application.h"

#include "graphics\opengl.h"

#include "debug\Debug.h"
#include "Input.h"
#include "graphics\Shader.h"

#include "graphics\DefaultRenderer.h"

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
    s_context->window->setVsync(false);

    s_context->targetFrameTime = 1.f / 60.f;
    s_context->maxUpdatesPerFrame = 5;
    s_context->totalDeltaTime = 0.f;
    s_context->deltaTime = 0.f;
    

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
      throw std::runtime_error("glew error: " + std::to_string(error) + ": " + std::string((const char*)glewGetErrorString(error)));
    }

    debug::Log("OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    debug::Log("GLSL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));
 
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    s_context->graphics.vao = std::make_unique<graphics::VertexArray>();

    s_context->graphics.renderer = std::make_unique<graphics::DefaultRenderer>();
    
    s_context->graphics.errorShader = graphics::Shader::Load("resources/shaders/error.shader");
    
    auto defaultShader = graphics::Shader::Load("resources/shaders/default.shader");
    s_context->graphics.defaultMaterial = graphics::Material::Create(defaultShader);
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
    s_context->totalDeltaTime += s_context->frameTime.getSeconds();
    s_context->frameTime.Reset();

    if (s_context->nextScene)
    {
      ChangeScene();
    }

    int count = 0;
    while (s_context->totalDeltaTime >= s_context->targetFrameTime)
    {
      s_context->deltaTime = s_context->targetFrameTime;
      HandleEvents();
      Update();
      s_context->totalDeltaTime -= s_context->targetFrameTime;

      if (++count > s_context->maxUpdatesPerFrame) { break; }
    }

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

    if (Input::getKeyDown(KeyCode::A))
    {
      debug::Log("A down");
    }

    if (Input::getKeyUp(KeyCode::A))
    {
      debug::Log("A up");
    }

    if (Input::getKey(KeyCode::A))
    {
      debug::Log("A");
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

      auto cameraData = camera->getCamera();

      s_context->graphics.renderer->Start(cameraData);

      if (s_context->scene)
      {
        s_context->scene->Render(*s_context->graphics.renderer);
      }

      s_context->graphics.renderer->End();

      s_context->graphics.renderer->Render();
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