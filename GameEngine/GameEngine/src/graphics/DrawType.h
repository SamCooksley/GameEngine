#ifndef _ENGINE_GRAPHICS_DRAWTYPE_H_
#define _ENGINE_GRAPHICS_DRAWTYPE_H_

#include "core/Types.h"
#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    struct DrawType
    {
    public:
      DrawType() = delete;

      using Type = uint8;

      static constexpr Type Points = 0;
      static constexpr Type Lines = 1;
      static constexpr Type Line_strip = 2;
      static constexpr Type Line_loop = 3;
      static constexpr Type Triangles = 4;
      static constexpr Type Triangle_strip = 5;
      static constexpr Type Triangle_fan = 6;

      static GLenum ToOpenGL(DrawType::Type _type);
    };

  }
}

#endif //_ENGINE_GRAPHICS_DRAWTYPE_H_