#include "stdafx.h"

#include "Engine_Time.h"

#include "Application.h"

namespace engine
{
  float Time::getDeltaTime()
  {
    return Application::s_context->deltaTime;
  }
}