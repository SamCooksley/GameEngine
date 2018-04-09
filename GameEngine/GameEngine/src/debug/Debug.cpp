#include "stdafx.h"

#include "Debug.h"

#include <iostream>

namespace engine
{
  namespace debug
  {
    void Log(const std::string & _message)
    {
      std::cout << _message << std::endl;
    }

    void LogWarning(const std::string & _message)
    {
      std::cout << "Warning: " << _message << std::endl;
    }

    void LogError(const std::string & _message)
    {
      std::cerr << "Error: " << _message << std::endl;
    }
  }
}