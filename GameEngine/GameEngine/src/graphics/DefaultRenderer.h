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
    DefaultRenderer(
      const std::shared_ptr<Material> & _deferred = Material::Create(Resources::Load<Shader>("resources/shaders/deferred_lighting.shader"))
    );
    ~DefaultRenderer();
  
    void Render() override;

    void Resize(uint _width, uint _height) override;
  
  private:
    void CreateGBuffer(uint _width, uint _height);
  
    std::shared_ptr<FrameBuffer> m_gBuffer;
  
    std::shared_ptr<Material> m_deferredMat;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_DEFAULTRENDERER_H_