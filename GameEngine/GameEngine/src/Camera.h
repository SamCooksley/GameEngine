#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include "Component.h"

#include "graphics\Camera.h"

#include "graphics\BaseRenderer.h"

#include "Scene.h"

namespace engine {

  enum class CameraType
  {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  class Camera : public Component
  {
    friend class Application;

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

    void SetupRender();

    static void AddCamera(const std::shared_ptr<Camera> & _camera);
    void RemoveCamera();

   private:
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

    std::shared_ptr<graphics::BaseRenderer> m_renderer;

    ENGINE_SETUPSHARED(Camera);
  };

} // engine

#endif //_ENGINE_CAMERA_H_