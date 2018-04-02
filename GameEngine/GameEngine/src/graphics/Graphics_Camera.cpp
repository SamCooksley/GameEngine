#include "stdafx.h"

#include "Graphics_Camera.h"

namespace engine
{
  namespace graphics
  {
    Camera::Camera() :
      projection(1.f),
      view(1.f),
      vp(1.f),
      position(0.f)
    { }

    Camera::Camera(
      const glm::mat4 & _projection,
      const glm::mat4 & _view, 
      const glm::vec3 & _position
    ) :
      projection(_projection),
      view(_view),
      vp(_projection * _view),
      position(_position)
    { }

    Camera::Camera(
      const glm::mat4 & _projection,
      const glm::mat4 & _view,
      const glm::mat4 & _vp,
      const glm::vec3 & _position
    ) :
      projection(_projection),
      view(_view),
      vp(_vp),
      position(_position)
    { }
  }
}