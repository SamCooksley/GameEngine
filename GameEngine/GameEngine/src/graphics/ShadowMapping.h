#ifndef _ENGINE_GRAPHICS_SHADOWMAPPING_H_
#define _ENGINE_GRAPHICS_SHADOWMAPPING_H_

#include "ShadowRenderer.h"
#include "FrameBuffer.h"

namespace engine { 
namespace graphics {
 
  class ShadowMapping
  {
  public:
    ShadowMapping(uint _width, uint _height, TextureFormat _format = TextureFormat::DEPTH_COMPONENT32F);
    ~ShadowMapping();

    const glm::mat4 & getLightSpace() const;

    const std::shared_ptr<Shadow2D> & getShadowMap() const;

    ShadowRenderer * getRenderer();

    void Setup(const Camera & _camera);

  private:
    glm::mat4 m_lightSpace;
    std::unique_ptr<ShadowRenderer> m_renderer;
    std::shared_ptr<FrameBuffer> m_frameBuffer;
    std::shared_ptr<Shadow2D> m_shadowMap;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOWMAPPING_H_