#ifndef _ENGINE_GRAPHICS_DEFAULTRENDERER_H_
#define _ENGINE_GRAPHICS_DEFAULTRENDERER_H_

#include "BaseRenderer.h"

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
    };
  }
}

#endif //_ENGINE_GRAPHICS_DEFAULTRENDERER_H_