#include "stdafx.h"

#include "Graphics.h"

#include "Application.h"

namespace engine
{
  graphics::UniformBuffer * Graphics::getUniformBuffer(const std::string & _name)
  {
    return Application::s_context->graphics.uniformBuffers.getBuffer(_name);
  }

  const std::shared_ptr<graphics::Material> & Graphics::getDefaultMaterial()
  {
    return Application::s_context->graphics.defaultMaterial;
  }

  graphics::GLData & Graphics::GL()
  {
    return *Application::s_context->graphics.glData;
  }

  std::shared_ptr<graphics::Shader> Graphics::getErrorShader()
  {
    return Application::s_context->graphics.errorShader;
  }

  std::shared_ptr<graphics::BaseRenderer> Graphics::getDefaultRenderer()
  {
    return Application::s_context->graphics.defaultRenderer;
  }

  void Graphics::AddCamera(std::shared_ptr<Camera> _camera)
  {
    if (!_camera) { return; }

    for (auto & cam : Application::s_context->graphics.cameras)
    {
      if (cam.lock() == _camera)
      {
        return;
      }
    }

    Application::s_context->graphics.cameras.push_back(_camera);
  }

  void Graphics::RemoveCamera(std::shared_ptr<Camera> _camera)
  {
    auto & cameras = Application::s_context->graphics.cameras;

    for (size_t i = 0; i < cameras.size();)
    {
      auto other = cameras[i].lock();
      if (!other || other == _camera)
      {
        cameras.erase(std::begin(cameras) + i);
      }
      else
      {
        ++i;
      }
    }
  }

  graphics::Context & Graphics::getContext()
  {
    return Application::s_context->graphics;
  }
}