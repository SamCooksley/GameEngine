#include "stdafx.h"

#include <iostream>

#include "Application.h"
#include "TestScene.h"

int main(int _argc, char** _argv)
{
  try
  {
    engine::Application::Init(_argc, _argv);

    engine::Scene::SetScene<engine::TestScene>();

    engine::Application::Loop();
  }
  catch (std::exception & _e)
  {
    engine::debug::LogError(_e.what());
    std::cin.get();
  }

  engine::Application::Exit();

  return 0;
}