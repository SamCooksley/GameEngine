#include "stdafx.h"  

#include "Application.h"

#include "glew\glew.h"

#include "debug\Debug.h"
#include "Input.h"

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

    debug::Log("OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));
    debug::Log("GLSL version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
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
    if (s_context->scene)
    {
      s_context->scene->Render();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    s_context->window->Present();
  }

  void Application::ChangeScene()
  {
    s_context->scene = s_context->nextScene;
    s_context->nextScene = nullptr;

    s_context->scene->Init();
  }
}