#include "stdafx.h"

#include "Camera.h"

#include "GameObject.h"

#include "graphics\Graphics.h"

#include "Application.h"

#include "graphics\DefaultRenderer.h"

namespace engine {

  Camera::Camera()
  { }

  Camera::~Camera()
  { }

  void Camera::OnAwake()
  {
    Component::OnAwake();

    setPerspective(60.f, 1.f, .01f, 100.f);

    m_renderer = std::make_unique<graphics::DefaultRenderer>();

    AddCamera();
  }

  void Camera::OnDestroy()
  {
    RemoveCamera();
  }

  graphics::Camera Camera::getCamera() const
  {
    auto transform = getGameObject()->getComponent<Transform>();
    
    glm::mat4 view(1.f);
    glm::vec3 pos(0.f);

    if (transform)
    {
      //remove scale from the transform by recreating the matrix.
      glm::quat rot;
      transform->get(&pos, &rot, nullptr);
      view = Transform::getTransform(pos, rot, glm::vec3(1.f));
      view = glm::inverse(view);
    }
    else
    {
      debug::LogError("Camera Error: " + getGameObject()->getName() + " missing transform component.");
    }

    return graphics::Camera(m_projection, view, pos);
  }

  void Camera::setPerspective(float _fov)
  {
    setPerspective(_fov, m_aspect, m_near, m_far);
  }

  void Camera::setPerspective(float _fov, float _aspect)
  {
    setPerspective(_fov, _aspect, m_near, m_far);
  }

  void Camera::setPerspective(float _fov, float _aspect, float _near, float _far)
  {
    m_type = CameraType::PERSPECTIVE;

    m_fov = _fov;
    m_aspect = _aspect;
    m_near = _near;
    m_far = _far;

    UpdateProjection();
  }

  void Camera::setOrthographic(float _size)
  {
    setOrthographic(_size, m_aspect, m_near, m_far);
  }

  void Camera::setOrthographic(float _size, float _aspect)
  {
    setOrthographic(_size, _aspect, m_near, m_far);
  }

  void Camera::setOrthographic(float _size, float _aspect, float _near, float _far)
  {
    m_type = CameraType::ORTHOGRAPHIC;

    m_size = _size;
    m_aspect = _aspect;
    m_near = _near;
    m_far = _far;

    UpdateProjection();
  }

  void Camera::setFOV(float _fov)
  {
    m_fov = _fov;
    UpdateProjection();
  }

  void Camera::setSize(float _size)
  {
    m_size = _size;
    UpdateProjection();
  }

  void Camera::setAspect(float _aspect)
  {
    m_aspect = _aspect;
    UpdateProjection();
  }

  void Camera::setZClipping(float _near, float _far)
  {
    m_near = _near;
    m_far = _far;

    UpdateProjection();
  }

  void Camera::UpdateProjection()
  {
    switch (m_type)
    {
      case CameraType::PERSPECTIVE:
      {
        m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
        break;
      }
      case CameraType::ORTHOGRAPHIC:
      {
        float x = m_size * m_aspect;
        m_projection = glm::ortho(-x, x, -m_size, m_size, m_near, m_far);
        break;
      }
    }
  }

  void Camera::SetupRender()
  {
    auto fb = Graphics::getContext().activeFrameBuffer.lock();

    uint width = fb->getWidth();
    uint height = fb->getHeight();

    setAspect(static_cast<float>(width) / static_cast<float>(height));

    m_renderer->Resize(width, height);
  }

  void Camera::AddCamera()
  {
    auto & cameras = Application::s_context->cameras;

    for (auto & cam : cameras)
    {
      if (cam.lock().get() == this)
      {
        return;
      }
    }

    cameras.push_back(Camera::getShared());
  }

  void Camera::RemoveCamera()
  {
    auto & cameras = Application::s_context->cameras;

    for (size_t i = 0; i < cameras.size();)
    {
      if (cameras[i].expired() || 
          cameras[i].lock().get() == this)
      {
        cameras.erase(std::begin(cameras) + i);
      }
      else { ++i; }
    }
  }

} // engine