#ifndef _ENGINE_GRAPHICS_CAMERACUBE_H_
#define _ENGINE_GRAPHICS_CAMERACUBE_H_

#include "Camera.h"

namespace engine {
namespace graphics {

  class CameraCube
  {
   public:
    CameraCube();
    CameraCube(const glm::vec3 & _position, float _near, float _far);

    const Camera & getCamera(size_t _i) const;

    const glm::vec3 & getPosition() const;

    float getNear() const;
    float getFar() const;

   private:
    std::array<Camera, 6> m_cameras;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_CAMERACUBE_H_