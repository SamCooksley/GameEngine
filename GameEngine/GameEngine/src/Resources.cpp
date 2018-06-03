#include "stdafx.h"

#include "Resources.h"

namespace engine {

  void Resources::Clear()
  {
    Application::s_context->resources.clear();
  }

} // engine