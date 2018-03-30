#include "stdafx.h"

#include "DrawType.h"

namespace engine
{
  namespace graphics
  {
    GLenum DrawType::ToOpenGL(DrawType::Type _type)
    {
      switch (_type)
      {
        case DrawType::Points:         { return GL_POINTS;         }
        case DrawType::Lines:          { return GL_LINES;          }
        case DrawType::Line_strip:     { return GL_LINE_STRIP;     }
        case DrawType::Line_loop:      { return GL_LINE_LOOP;      }
        case DrawType::Triangles:      { return GL_TRIANGLES;      }
        case DrawType::Triangle_strip: { return GL_TRIANGLE_STRIP; }
        case DrawType::Triangle_fan:   { return GL_TRIANGLE_FAN;   }
        default: { return 0; }
      }
    }
  }
}