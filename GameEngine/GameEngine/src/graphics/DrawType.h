#ifndef _ENGINE_GRAPHICS_DRAWTYPE_H_
#define _ENGINE_GRAPHICS_DRAWTYPE_H_

#include "core/Types.h"
#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    enum class DrawType : GLenum
    {
      POINTS = GL_POINTS,
      LINES = GL_LINES,
      LINE_STRIP = GL_LINE_STRIP,
      LINE_LOOP = GL_LINE_LOOP,
      TRIANGLES = GL_TRIANGLES,
      TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
      TRIANGLE_FAN = GL_TRIANGLE_FAN
    };

    GLenum DrawTypeToOpenGL(DrawType _type);
    DrawType OpenGLToDrawType(GLenum _type);
  }
}

#endif //_ENGINE_GRAPHICS_DRAWTYPE_H_