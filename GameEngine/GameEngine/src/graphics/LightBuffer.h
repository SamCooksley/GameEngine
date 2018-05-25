#ifndef _ENGINE_GRAPHICS_LIGHTBUFFER_H_
#define _ENGINE_GRAPHICS_LIGHTBUFFER_H_

#include "Graphics_Light.h"

#include "UniformBuffer.h"

namespace engine
{
  namespace graphics
  {
    class LightBuffer : public UniformBuffer
    {
    public:
      static const std::string name; 

      static const uint max_lights;

    private:
      static const uint
        type_offset,
        position_offset,
        direction_offset,
        colour_offset,
        linear_offset,
        quadratic_offset,
        cutoff_offset,
        outercutoff_offset,

        light_size,
        lights_size,

        ambient_offset,

        size;

    public:
      LightBuffer();
      ~LightBuffer();

      void setLight(const Light & _light, uint _num);
      void ClearLight(uint _num);
      void setAmbient(const glm::vec3 & _ambient);
    };
  }
}

#endif //_ENGINE_GRAPHICS_LIGHTBUFFER_H_