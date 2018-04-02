#include "stdafx.h"

#include "Graphics.h"

#include "Application.h"

namespace engine
{
  std::shared_ptr<graphics::Material> Graphics::getDefaultMaterial()
  {
    return Application::s_context->graphics.defaultMaterial;
  }

  std::shared_ptr<graphics::Shader> Graphics::getErrorShader()
  {
    return Application::s_context->graphics.errorShader;
  }
}