#include "stdafx.h"

#include "eTime.h"

#include "Application.h"

namespace engine {

  float Time::getDeltaTime()
  {
    return Application::s_context->deltaTime;
  }

} // engine