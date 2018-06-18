#ifndef _ENGINE_GRAPHICS_CSMRENDERER_H_
#define _ENGINE_GRAPHICS_CSMRENDERER_H_

#include "ShadowRenderer.h"

namespace engine {
namespace graphics {

  class CSMRenderer : public ShadowRenderer
  {
   public:
    static const uint NUM_CASCADES = 4u;

   public:
    void setDirectional(const glm::vec3 & _direction, const Camera & _target) override;

    void Render(const ShadowCommandBuffer & _occluders) override;
   
   private:
    std::vector<Camera> m_camera;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_CSMRENDERER_H_
