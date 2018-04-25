#include "stdafx.h"

#include "DrawType.h"

namespace engine
{
  namespace graphics
  {
    GLenum DrawTypeToOpenGL(DrawType _type)
    {
      return (GLenum)_type;
    }

    DrawType OpenGLToDrawType(GLenum _type)
    {
      switch (_type)
      {
        case GL_POINTS:         { return DrawType::POINTS;         }
        case GL_LINES:          { return DrawType::LINES;          }
        case GL_LINE_STRIP:     { return DrawType::LINE_STRIP;     }
        case GL_LINE_LOOP:      { return DrawType::LINE_LOOP;      }
        case GL_TRIANGLES:      { return DrawType::TRIANGLES;      }
        case GL_TRIANGLE_STRIP: { return DrawType::TRIANGLE_STRIP; }
        case GL_TRIANGLE_FAN:   { return DrawType::TRIANGLE_FAN;   }
        default: { assert(false); }
      }

      return DrawType::POINTS;
    }
  }
}