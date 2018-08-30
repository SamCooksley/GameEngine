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

#include "EnvironmentCaptureFactory.h"

#include "filter\FilterCube.h"

#include "SSAO.h"

namespace engine {
namespace graphics {

  class DefaultRenderer : public BaseRenderer
  {
   public:
    DefaultRenderer();
    ~DefaultRenderer();
  
    void Render() override;

    void Resize(int _width, int _height) override;
  
    void setSkybox(const std::shared_ptr<Skybox> & _skybox) override;

   private:
    void CreateGBuffer(int _width, int _height);
    void CreateTarget(int _width, int _height);

    void GenerateShadows();
    void GeneratePerCameraShadows();
    void DeferredRender();
    void ForwardRender();
    void PostProcess();
   
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

    std::unique_ptr<EnvironmentCaptureFactory> m_environmentFactory;

    std::shared_ptr<FrameBuffer> m_target;

    std::shared_ptr<Shader> m_hdr;

    std::unique_ptr<EnvironmentCapture> m_environment;

    std::shared_ptr<Texture2D> m_brdfLUT;

    std::unique_ptr<SSAO> m_ssaoProcess;
    std::unique_ptr<Texture2D>  m_ssao;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_DEFAULTRENDERER_H_