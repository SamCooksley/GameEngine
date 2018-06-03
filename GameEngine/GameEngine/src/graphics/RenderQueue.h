#ifndef _ENGINE_GRAPHICS_RENDERQUEUE_H_
#define _ENGINE_GRAPHICS_RENDERQUEUE_H_

#include "core\String.h"

namespace engine {
namespace graphics {

  class RenderQueue
  {
   public:
    RenderQueue() = delete;
  
    enum Queue { DEFERRED, FORWARD, TRANSPARENT, COUNT };
  
    static const String & ToString(Queue _queue);
    static Queue FromString(const String & _s);
  
   private:
    static const std::array<String, COUNT> s_names;
  };
 
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_RENDERQUEUE_H_