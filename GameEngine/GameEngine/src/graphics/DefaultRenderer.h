#ifndef _ENGINE_GRAPHICS_DEFAULTRENDERER_H_
#define _ENGINE_GRAPHICS_DEFAULTRENDERER_H_

#include "BaseRenderer.h"

#include "FrameBuffer.h"

namespace engine
{
  namespace graphics
  {
    class DefaultRenderer : public BaseRenderer
    {
    public:
      DefaultRenderer();
      ~DefaultRenderer();

      void Render() override;

    private:
      void CreateGBuffer(uint _width, uint _height);

      std::unique_ptr<FrameBuffer> m_gBuffer;

      std::shared_ptr<Material> m_differedMat;
    };
  }
}

#endif //_ENGINE_GRAPHICS_DEFAULTRENDERER_H_