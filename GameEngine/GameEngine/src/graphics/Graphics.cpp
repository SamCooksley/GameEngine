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
}