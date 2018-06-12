#ifndef _ENGINE_GRAPHICS_CAMERA_H_
#define _ENGINE_GRAPHICS_CAMERA_H_

#include "core\Types.h"

namespace engine {
namespace graphics {

  struct Camera
  {
   public:
    Camera();
    Camera(
      const glm::mat4 & _projection,
      const glm::mat4 & _view, 
      const glm::vec3 & _position
    );
    Camera(
      const glm::mat4 & _projection, 
      const glm::mat4 & _view, 
      const glm::mat4 & _vp, 
      const glm::vec3 & _position
    );

   public:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 vp;
  
    glm::vec3 position;
  };
  
} }

#endif //_ENGINE_GRAPHICS_CAMERA_H_