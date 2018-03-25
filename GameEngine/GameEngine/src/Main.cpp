#include "stdafx.h"

#include <iostream>

#include "Application.h"
#include "debug\Debug.h"

int main(int _argc, char** _argv)
{
  try
  {
    engine::Application::Init(_argc, _argv);
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