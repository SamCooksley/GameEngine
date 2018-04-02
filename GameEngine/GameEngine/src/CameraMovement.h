#ifndef _ENGINE_CAMERAMOVEMENT_H_
#define _ENGINE_CAMERAMOVEMENT_H_

#include "Component.h"

namespace engine
{
  class CameraMovement : public Component
  {
  public:
    CameraMovement();
    ~CameraMovement();

  protected:
    void OnStart() override;
    void OnUpdate() override;

  private:
    float m_moveSpeed;
    float m_rotationSpeed;

    float m_yaw, m_pitch;

    glm::vec2 m_prevMouse;
  };
}

#endif //_ENGINE_CAMERAMOVEMENT_H_