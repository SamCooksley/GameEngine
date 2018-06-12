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

    glm::mat4 proj(1.0f);

    switch (m_type)
    {
      case CameraType::PERSPECTIVE:
      {
        proj = glm::perspective(m_fov, _aspect, m_near, m_far);
        break;
      }
      case CameraType::ORTHOGRAPHIC:
      {
        float x = m_size * _aspect;
        proj = glm::ortho(-x, x, -m_size, m_size, m_near, m_far);
        break;
      }
    }

    glm::vec3 pos; glm::quat rot;
    transform->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    return graphics::Camera(proj, view, pos);
  }

  void Camera::setPerspective(float _fov)
  {
    setPerspective(_fov, m_near, m_far);
  }

  void Camera::setPerspective(float _fov, float _near, float _far)
  {
    m_type = CameraType::PERSPECTIVE;

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
    m_type = CameraType::ORTHOGRAPHIC;

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

  std::vector<glm::vec3> Camera::getFrustumPoints() const
  {
    float distances[2] = { 0.f, 1.f };
    return getFrustumPoints(distances, 2);
  }

  std::vector<glm::vec3> Camera::getFrustumPoints(const float * _distances, size_t _size) const
  {
    float fovy = glm::radians(m_fov * 0.5f);
    //TODO: aspect
    float fovx = fovy * 1.f;//1.33333f; //* m_aspect;

    float tay = glm::tan(fovy);
    float tax = glm::tan(fovx);

    const std::array<glm::vec2, 4u> dir = {
      glm::vec2(-1.f, -1.f),
      glm::vec2( 1.f, -1.f),
      glm::vec2( 1.f,  1.f),
      glm::vec2(-1.f,  1.f)
    };

    std::vector<glm::vec3> results(_size * 4u);

    for (size_t i = 0u; i < _size; ++i)
    {
      for (size_t j = 0u; j < dir.size(); ++j)
      {
        float dist = m_near + (m_far - m_near) * _distances[i];

        results[i * 4u + j] = glm::vec3(
          tax * dist * dir[j].x,
          tay * dist * dir[j].y,
          dist * -1.f // invert distance as camera forward is negative
        );
      }
    }

    return results;
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