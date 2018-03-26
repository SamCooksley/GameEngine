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
    s_context->window->setVsync(true);

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

    while (s_context->state == core::EngineState::Running)
    {
      HandleEvents();
      Update();
      Render();
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

  void Application::HandleEvents()
  {
    Input::Reset();
    s_context->glfwContext->PollEvents();
  }

  void Application::Update()
  {
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    s_context->window->Present();
  }
}