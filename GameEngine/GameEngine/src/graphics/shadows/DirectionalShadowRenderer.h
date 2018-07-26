#ifndef _ENGINE_GRAPHICS_DIRECTIONALSHADOWRENDERER_H_
#define _ENGINE_GRAPHICS_DIRECTIONALSHADOWRENDERER_H_

#include "Camera.h"

#include "..\FrameBuffer.h"

namespace engine {
namespace graphics { 

  class DirectionalShadowRenderer
  {
  public:
    DirectionalShadowRenderer();
    ~DirectionalShadowRenderer();

    virtual void setLight(const glm::vec3 & _direction, const Camera & _camera);

    virtual void Render() = 0;

  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_DIRECTIONALSHADOWRENDERER_H_