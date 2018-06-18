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

    m_target = Graphics::getContext().defaultFrameBuffer;

    setPerspective(60.f, .01f, 100.f);

    assert(!Application::s_context->camera && "Only one camera supported");
    setAsMainCamera();
  }

  void Camera::OnDestroy()
  {
    RemoveAsMainCamera();
  }

  graphics::Camera Camera::getCamera(float _aspect) const
  {
    auto transform = getGameObject()->getComponent<Transform>();
    assert(transform);

    glm::vec3 pos; glm::quat rot;
    transform->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    return graphics::Camera(m_type, m_fov, _aspect, m_near, m_far, view, pos);
  }

  void Camera::setPerspective(float _fov)
  {
    setPerspective(_fov, m_near, m_far);
  }

  void Camera::setPerspective(float _fov, float _near, float _far)
  {
    m_type = graphics::CameraType::PERSPECTIVE;

    m_fov = _fov;
    m_near = _near;
    m_far = _far;
  }

  void Camera::setOrthographic(float _size)
  {
    setOrthographic(_size, m_near, m_far);
  }

  void Camera::setOrthographic(float _size, float _near, float _far)
  {
    m_type = graphics::CameraType::ORTHOGRAPHIC;

    m_size = _size;
    m_near = _near;
    m_far = _far;
  }

  void Camera::setZClipping(float _near, float _far)
  {
    m_near = _near;
    m_far = _far;
  }

  graphics::FrameBuffer & Camera::getRenderTarget()
  {
    return *m_target;
  }

  void Camera::setAsMainCamera()
  {
    Application::s_context->camera = Camera::getShared();
  }

  void Camera::RemoveAsMainCamera()
  {
    if (Application::s_context->camera.get() == this)
    {
      Application::s_context->camera = nullptr;
    }
  }

} // engine