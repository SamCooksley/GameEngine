#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "Application.h"
#include "TestScene.h"
#include "debug\Debug.h"

#include "graphics\ShaderParser.h"

int main(int _argc, char** _argv)
{
  try
  {
    std::string path("resources/shaders/test.shader");
    std::ifstream file(path);
    engine::graphics::ShaderParser shader(path, file);

    if (shader.HasShader(engine::graphics::ShaderType::VERTEX))
    {
      engine::debug::Log("Vertex");
      engine::debug::Log(shader.GetShaderSource(engine::graphics::ShaderType::VERTEX));
    }

    if (shader.HasShader(engine::graphics::ShaderType::FRAGMENT))
    {
      engine::debug::Log("Fragment");
      engine::debug::Log(shader.GetShaderSource(engine::graphics::ShaderType::FRAGMENT));
    }

    if (shader.HasShader(engine::graphics::ShaderType::GEOMETRY))
    {
      engine::debug::Log("Geometry");
      engine::debug::Log(shader.GetShaderSource(engine::graphics::ShaderType::GEOMETRY));
    }

    engine::Application::Init(_argc, _argv);

    engine::Scene::SetScene<engine::TestScene>();

    engine::Application::Loop();
    engine::Application::Exit();
  }
  catch (std::exception& _e)
  {
    engine::debug::LogError(_e.what());
    std::cin.get();
  }
  return 0;
}