#include "stdafx.h"

#include "Graphics.h"

#include "Application.h"

namespace engine
{
  graphics::UniformBuffer * Graphics::getUniformBuffer(const std::string & _name)
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

  std::shared_ptr<graphics::Shader> Graphics::getErrorShader()
  {
    return getContext().errorShader;
  }

  graphics::Context & Graphics::getContext()
  {
    assert(Application::s_context && Application::s_context->graphics);
    return *Application::s_context->graphics;
  }
}