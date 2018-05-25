#ifndef _ENGINE_GRAPHICS_LIGHT_H_
#define _ENGINE_GRAPHICS_LIGHT_H_

#include "core\Types.h"

namespace engine
{
  namespace graphics
  {
    enum class LightType : int
    {
      None = -1,
      Directional = 0,
      Point = 1,
      Spot = 2
    };

    struct Light
    {
    public:
      LightType type;
      glm::vec3 position;
      glm::vec3 direction;
      glm::vec3 colour;

      float linear;
      float quadratic;

      float cutoff;
      float outerCutoff;
    };
  }
}

#endif //_ENGINE_GRAPHICS_LIGHT_H_