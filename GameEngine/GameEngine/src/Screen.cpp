#include "stdafx.h"

#include "Screen.h"

#include "Application.h"

namespace engine {

  uint Screen::getWidth()
  {
    return Application::s_context->window->getWidth();
  }

  uint Screen::getHeight()
  {
    return Application::s_context->window->getHeight();
  }

} // engine