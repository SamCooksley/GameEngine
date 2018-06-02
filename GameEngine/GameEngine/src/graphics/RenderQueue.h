#ifndef _ENGINE_GRAPHICS_RENDERQUEUE_H_
#define _ENGINE_GRAPHICS_RENDERQUEUE_H_

#include "core\String.h"

namespace engine
{
  namespace graphics
  {
    class RenderQueue
    {
    public:
      RenderQueue() = delete;

      enum Queue { DEFERRED, FORWARD, TRANSPARENT, COUNT };

      static const std::string & ToString(Queue _queue);
      static Queue FromString(const std::string & _s);

    private:
      static const std::array<std::string, COUNT> s_names;
    };
  }
}

#endif //_ENGINE_GRAPHICS_RENDERQUEUE_H_