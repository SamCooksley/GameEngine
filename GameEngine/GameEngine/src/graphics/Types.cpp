#include "stdafx.h"

#include "Types.h"

namespace engine
{
  namespace graphics
  {
    GLenum TypeToOpenGL(Type _type)
    {
      return (GLenum)_type;
    }

    Type OpenGLToType(GLenum _type)
    {
      switch (_type)
      {
        case GL_INT:        { return Type::INT;   }
        case GL_FLOAT:      { return Type::FLOAT; }
        case GL_FLOAT_VEC2: { return Type::VEC2;  }
        case GL_FLOAT_VEC3: { return Type::VEC3;  }
        case GL_FLOAT_VEC4: { return Type::VEC4;  }
        case GL_FLOAT_MAT3: { return Type::MAT3;  }
        case GL_FLOAT_MAT4: { return Type::MAT4;  }
        default: { assert(false); }
      }

      return Type::INT;
    }

    GLenum ComponentTypeToOpenGL(ComponentType _type)
    {
      return (GLenum)_type;
    }

    ComponentType OpenGLToComponentType(GLenum _type)
    {
      switch (_type)
      {
        case GL_INT:   { return ComponentType::INT;   }
        case GL_FLOAT: { return ComponentType::FLOAT; }
        default: { assert(false); }
      }

      return ComponentType::INT;
    }
  }
}