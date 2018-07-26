#ifndef _ENGINE_GRAPHICS_DEFAULTRENDERER_H_
#define _ENGINE_GRAPHICS_DEFAULTRENDERER_H_

#include "BaseRenderer.h"

#include "FrameBuffer.h"

#include "Resources.h"

#include "ShadowRenderer.h"

#include "shadows\CascadedShadowRenderer.h"
#include "shadows\PointShadowRendererDP.h"
#include "shadows\PointShadowRendererCube.h"
#include "shadows\SpotShadowRenderer.h"

namespace engine {
namespace graphics {

  class DefaultRenderer : public BaseRenderer
  {
   public:
    DefaultRenderer();
    ~DefaultRenderer();
  
    void Render() override;

    void Resize(int _width, int _height) override;
  
   private:
    void CreateGBuffer(int _width, int _height);

    void GenerateShadows();
    void GeneratePerCameraShadows();
    void DeferredRender();
    void ForwardRender();
   
   private:
    std::shared_ptr<FrameBuffer> m_gBuffer;

    std::shared_ptr<Texture2D> m_position;
    std::shared_ptr<Texture2D> m_normal;
    std::shared_ptr<Texture2D> m_colour;

    std::shared_ptr<Shader> m_deferredAmbient;
    std::shared_ptr<Shader> m_deferredDirectional;
    std::shared_ptr<Shader> m_deferredPoint;
    std::shared_ptr<Shader> m_deferredSpot;

    std::unique_ptr<CascadedShadowRenderer> m_directionalShadowRenderer;
    std::unique_ptr<PointShadowRendererCube> m_pointShadowRenderer;
    std::unique_ptr<SpotShadowRenderer> m_spotShadowRenderer;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_DEFAULTRENDERER_H_