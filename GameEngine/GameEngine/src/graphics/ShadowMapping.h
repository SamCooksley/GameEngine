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

    const std::shared_ptr<Texture2D> & getShadowMap() const;

    void Setup();

  private:
    std::unique_ptr<ShadowRenderer> m_renderer;
    std::shared_ptr<FrameBuffer> m_frameBuffer;
    std::shared_ptr<Texture2D> m_shadowMap;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOWMAPPING_H_