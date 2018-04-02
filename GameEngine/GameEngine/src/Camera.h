#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include "Component.h"

#include "graphics\Graphics_Camera.h"

namespace engine
{
  enum class CameraType
  {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  class Camera : public Component
  {
  public:
    Camera();
    ~Camera();

    graphics::Camera getCamera() const;

    void setPerspective(float _fov);
    void setPerspective(float _fov, float _aspect);
    void setPerspective(float _fov, float _aspect, float _near, float _far);

    void setOrthographic(float _size);
    void setOrthographic(float _size, float _aspect);
    void setOrthographic(float _size, float _aspect, float _near, float _far);

    void setFOV(float _fov);
    void setSize(float _size);

    void setAspect(float _aspect);

    void setZClipping(float _near, float _far);

  protected:
    void OnAwake() override;
    void OnDestroy() override;

  private:
    void UpdateProjection();

    CameraType m_type;

    union
    {
      float m_fov;
      float m_size;
    };

    float m_aspect;

    float m_near;
    float m_far;

    glm::mat4 m_projection;

    ENGINE_SETUPSHARED(Camera);
  };
}

#endif //_ENGINE_CAMERA_H_