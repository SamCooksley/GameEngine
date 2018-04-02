#include "stdafx.h"

#include "CameraMovement.h"

#include "GameObject.h"

#include "Input.h"
#include "Engine_Time.h"

namespace engine
{
  CameraMovement::CameraMovement()
  { }

  CameraMovement::~CameraMovement()
  { }

  void CameraMovement::OnStart()
  {
    m_prevMouse = Input::getMousePosition();

    m_moveSpeed = 1.f;
    m_rotationSpeed = .01f;

    m_yaw = m_pitch = 0.f;

    Input::setCursorMode(CursorMode::FIXED);
  }

  void CameraMovement::OnUpdate()
  {
    glm::vec3 move;
    if (Input::getKey(KeyCode::W))
    {
      move.z -= m_moveSpeed;
    }
    if (Input::getKey(KeyCode::S))
    {
      move.z += m_moveSpeed;
    }

    if (Input::getKey(KeyCode::A))
    {
      move.x -= m_moveSpeed;
    }
    if (Input::getKey(KeyCode::D))
    {
      move.x += m_moveSpeed;
    }

    move * Time::getDeltaTime();

    auto transform = getGameObject()->getComponent<Transform>();
    glm::vec3 position = transform->getLocalPosition();
    position += transform->getRight() * move.x + transform->getForward() * move.z;
    transform->setLocalPosition(position);

    glm::vec2 mousePos = Input::getMousePosition();
    glm::vec2 delta = m_prevMouse - mousePos;
    m_prevMouse = mousePos;

    if (!Input::getKey(KeyCode::LEFT_CONTROL))
    {
      Input::setCursorMode(CursorMode::FIXED);

      m_yaw += delta.x * m_rotationSpeed;
      m_pitch += delta.y * m_rotationSpeed;
    }
    else
    {
      Input::setCursorMode(CursorMode::FREE);
    }

    m_pitch = glm::clamp(m_pitch, -1.5f, 1.5f);

    transform->setRotation(glm::quat(glm::vec3(m_pitch, m_yaw, 0.f)));  
  }
}