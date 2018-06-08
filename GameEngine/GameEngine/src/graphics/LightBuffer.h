#ifndef _ENGINE_GRAPHICS_LIGHTBUFFER_H_
#define _ENGINE_GRAPHICS_LIGHTBUFFER_H_

#include "Lights.h"

#include "UniformBuffer.h"

namespace engine {
namespace graphics {
    
  class LightBuffer : public UniformBuffer
  {
   public:
    static const String NAME; 
  
    static const uint
      MAX_DIRECTIONAL,
      MAX_POINT,
      MAX_SPOT;

   private:
    static const uint
      NUMDIR_OFFSET,
      NUMPOINT_OFFSET,
      NUMSPOT_OFFSET,
  
      AMBIENT_OFFSET,

      DIRECTIONAL_OFFSET,
      POINT_OFFSET,
      SPOT_OFFSET,

      SIZE;

    struct Directional
    {
      static const uint
        COLOUR_OFFSET,
        DIRECTION_OFFSET,
        
        SIZE;
    };

    struct Point
    {
      static const uint
        COLOUR_OFFSET,
        POSITION_OFFSET,

        LINEAR_OFFSET,
        QUADRATIC_OFFSET,

        SIZE;
    };

    struct Spot
    {
      static const uint
        COLOUR_OFFSET,
        POSITION_OFFSET,
        DIRECTION_OFFSET,

        CUTOFF_OFFSET,
        OUTERCUTOFF_OFFSET,

        LINEAR_OFFSET,
        QUADRATIC_OFFSET,

        SIZE;
    };
  
   public:
    LightBuffer();
    ~LightBuffer();
  
    void setLights(const Lights & _lights);
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_LIGHTBUFFER_H_