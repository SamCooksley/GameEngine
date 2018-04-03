#ifndef _ENGINE_SCREEN_H_
#define _ENGINE_SCREEN_H_

#include "core\Types.h"

namespace engine
{
  class Screen
  {
  public:
    Screen() = delete;

    static uint getWidth();
    static uint getHeight();
  };
}

#endif //_ENGINE_SCREEN_H_