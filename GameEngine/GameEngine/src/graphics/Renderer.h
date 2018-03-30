#ifndef _ENGINE_GRAPHICS_RENDERER_H_
#define _ENGINE_GRAPHICS_RENDERER_H_

#include "Camera.h"

namespace engine
{
  namespace graphics
  {
    class Renderer
    {
    public:
      virtual ~Renderer();

    protected:
      Renderer();
    private:
    };
  }
}

#endif //_ENGINE_GRAPHICS_RENDERER_H_