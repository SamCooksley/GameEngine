#include "stdafx.h"

#include "TypeData.h"

namespace engine
{
  namespace graphics
  {
    GLenum GetComponentType(GLenum _type)
    {
      switch (_type)
      {
        case GL_INT:        { return TypeData<int>::component;       }
        case GL_FLOAT:      { return TypeData<float>::component;     }
        case GL_FLOAT_VEC2: { return TypeData<glm::vec2>::component; }
        case GL_FLOAT_VEC3: { return TypeData<glm::vec3>::component; }
        case GL_FLOAT_VEC4: { return TypeData<glm::vec4>::component; }
        case GL_FLOAT_MAT3: { return TypeData<glm::mat3>::component; }
        case GL_FLOAT_MAT4: { return TypeData<glm::mat4>::component; }
        default: { return 0; }
      }
    }

    uint GetComponentCount(GLenum _type)
    {
      switch (_type)
      {
        case GL_INT:        { return TypeData<int>::count;       }
        case GL_FLOAT:      { return TypeData<float>::count;     }
        case GL_FLOAT_VEC2: { return TypeData<glm::vec2>::count; }
        case GL_FLOAT_VEC3: { return TypeData<glm::vec3>::count; }
        case GL_FLOAT_VEC4: { return TypeData<glm::vec4>::count; }
        case GL_FLOAT_MAT3: { return TypeData<glm::mat3>::count; }
        case GL_FLOAT_MAT4: { return TypeData<glm::mat4>::count; }
        default: { return 0; }
      }
    }

    uint GetTypeSize(GLenum _type)
    {
      switch (_type)
      {
        case GL_INT:        { return sizeof(int);       }
        case GL_FLOAT:      { return sizeof(float);     }
        case GL_FLOAT_VEC2: { return sizeof(glm::vec2); }
        case GL_FLOAT_VEC3: { return sizeof(glm::vec3); }
        case GL_FLOAT_VEC4: { return sizeof(glm::vec4); }
        case GL_FLOAT_MAT3: { return sizeof(glm::mat3); }
        case GL_FLOAT_MAT4: { return sizeof(glm::mat4); }
        default: { return 0; }
      }
    }
  }
}