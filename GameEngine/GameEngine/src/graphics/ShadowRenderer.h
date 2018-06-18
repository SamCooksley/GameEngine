#ifndef _ENGINE_GRAPHICS_SHADOWRENDERER_H_
#define _ENGINE_GRAPHICS_SHADOWRENDERER_H_

#include "Renderer.h"

#include "CommandBuffer.h"

#include "Camera.h"

namespace engine {
namespace graphics {

  class ShadowRenderer
  {
   public:
    ShadowRenderer(const std::shared_ptr<Shader> & _depth);
    ~ShadowRenderer();

    virtual void setDirectional(const glm::vec3 & _direction);
    virtual void setDirectional(const glm::vec3 & _direction, const Camera & _target);

    virtual void setPoint(const glm::vec3 & _position, float _radius);

    virtual void setSpot(const glm::vec3 & _position, const glm::vec3 & _direction, float _fov, float _distance);

    virtual void Render(const ShadowCommandBuffer & _occluders) = 0;
  
   protected:
    void RenderCommands(const ShadowCommandBuffer & _occluders);

   protected:
    std::shared_ptr<Shader> m_depth;
  };

} } // engine::grahics

#endif //_ENGINE_GRAPHICS_SHADOWRENDERER_H_