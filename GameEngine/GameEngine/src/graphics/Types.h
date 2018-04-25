#ifndef _ENGINE_GRAPHICS_TYPES_H_
#define _ENGINE_GRAPHICS_TYPES_H_

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    enum class Type : GLenum
    {
      INT = GL_INT,
      FLOAT = GL_FLOAT,
      VEC2 = GL_FLOAT_VEC2,
      VEC3 = GL_FLOAT_VEC3,
      VEC4 = GL_FLOAT_VEC4,
      MAT3 = GL_FLOAT_MAT3,
      MAT4 = GL_FLOAT_MAT4
    };

    GLenum TypeToOpenGL(Type _type);
    Type OpenGLToType(GLenum _type);

    enum class ComponentType : GLenum
    {
      INT = GL_INT,
      FLOAT = GL_FLOAT
    };

    GLenum ComponentTypeToOpenGL(ComponentType _type);
    ComponentType OpenGLToComponentType(GLenum _type);
  }
}

#endif //_ENGINE_GRAPHICS_TYPES_H_