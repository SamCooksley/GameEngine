#include "stdafx.h"

#include "Debug.h"

#include <iostream>

namespace engine {
namespace debug {

  void Log(const String & _message)
  {
    std::cout << _message << std::endl;
  }
  
  void LogWarning(const String & _message)
  {
    std::cout << "Warning: " << _message << std::endl;
  }
  
  void LogError(const String & _message)
  {
    std::cerr << "Error: " << _message << std::endl;
  }
  
} } // engine::debug