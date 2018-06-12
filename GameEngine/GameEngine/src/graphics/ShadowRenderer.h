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
    ShadowRenderer(
      const std::shared_ptr<Shader> & _depth = Shader::Load("resources/shaders/depth.shader")
    );
    ~ShadowRenderer();

    void Render(const Camera & _camera, const ShadowCommandBuffer & _occluders);

   private:
    std::shared_ptr<Shader> m_depth;
  };

} } // engine::grahics

#endif //_ENGINE_GRAPHICS_SHADOWRENDERER_H_