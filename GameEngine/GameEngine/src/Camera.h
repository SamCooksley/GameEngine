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
   public:
    Camera();
    ~Camera();

    graphics::Camera getCamera(float _aspect) const;

    void setPerspective(float _fov);
    void setPerspective(float _fov, float _near, float _far);

    void setOrthographic(float _size);
    void setOrthographic(float _size, float _near, float _far);

    void setFOV(float _fov);
    void setSize(float _size);

    void setZClipping(float _near, float _far);

    graphics::FrameBuffer & getRenderTarget();

   protected:
    void OnAwake() override;
    void OnDestroy() override;

   private:
    void setAsMainCamera();
    void RemoveAsMainCamera();

   private:
    CameraType m_type;

    union
    {
      float m_fov;
      float m_size;
    };

    float m_near;
    float m_far;

    std::shared_ptr<graphics::FrameBuffer> m_target; //TODO

    ENGINE_SETUPSHARED(Camera);
  };

} // engine

#endif //_ENGINE_CAMERA_H_