#include "stdafx.h"

#include "System.h"

#include <Windows.h>

namespace engine {
namespace utilities {

  void Sleep(unsigned long _miliseconds)
  {
    ::Sleep(_miliseconds);
  }
 
} } // engine::utilities