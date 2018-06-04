#include "stdafx.h"

#include "Graphics.h"

#include "Application.h"

namespace engine {

  graphics::UniformBuffer * Graphics::getUniformBuffer(const String & _name)
  {
    return getContext().uniformBuffers.getBuffer(_name);
  }

  const std::shared_ptr<graphics::Material> & Graphics::getDefaultMaterial()
  {
    return getContext().defaultMaterial;
  }

  graphics::GLData & Graphics::GL()
  {
    return getContext().glData;
  }

  graphics::Context & Graphics::getContext()
  {
    assert(Application::s_context && Application::s_context->graphics);
    return *Application::s_context->graphics;
  }

} // engine