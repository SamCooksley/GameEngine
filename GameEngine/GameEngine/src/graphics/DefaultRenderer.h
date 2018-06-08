#ifndef _ENGINE_GRAPHICS_DEFAULTRENDERER_H_
#define _ENGINE_GRAPHICS_DEFAULTRENDERER_H_

#include "BaseRenderer.h"

#include "FrameBuffer.h"

#include "Resources.h"

namespace engine {
namespace graphics {

  class DefaultRenderer : public BaseRenderer
  {
  public:
    DefaultRenderer();
    ~DefaultRenderer();
  
    void Render() override;

    void Resize(uint _width, uint _height) override;
  
  private:
    void CreateGBuffer(uint _width, uint _height);
  
    std::shared_ptr<FrameBuffer> m_gBuffer;

    std::shared_ptr<Texture2D> m_position;
    std::shared_ptr<Texture2D> m_normal;
    std::shared_ptr<Texture2D> m_colour;

    std::shared_ptr<Shader> m_deferredAmbient;
    std::shared_ptr<Shader> m_deferredDirectional;
    std::shared_ptr<Shader> m_deferredPoint;
    std::shared_ptr<Shader> m_deferredSpot;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_DEFAULTRENDERER_H_