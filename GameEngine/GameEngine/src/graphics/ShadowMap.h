#ifndef _ENGINE_GRAPHICS_SHADOWMAP_H_
#define _ENGINE_GRAPHICS_SHADOWMAP_H_

#include "ShadowRenderer.h"
#include "FrameBuffer.h"

namespace engine { 
namespace graphics {
 
  class ShadowMap
  {
  public:
    ShadowMap(
      uint _width, uint _height,
      TextureFormat _format = TextureFormat::DEPTH_COMPONENT32F, 
      const std::shared_ptr<Shader> & _depth = nullptr
    );
    ~ShadowMap();

    const glm::mat4 & getLightSpace() const;

    const std::shared_ptr<Shadow2D> & getShadowMap() const;

    void GenerateShadowMap(const Camera & _camera, const ShadowCommandBuffer & _occluders);

  private:
    glm::mat4 m_lightSpace;
    std::shared_ptr<Shader> m_depth;
    std::shared_ptr<FrameBuffer> m_frameBuffer;
    std::shared_ptr<Shadow2D> m_shadowMap;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOWMAPPING_H_