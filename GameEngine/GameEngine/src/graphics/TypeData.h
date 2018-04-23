#ifndef _ENGINE_GRAPHICS_TYPEDATA_H_
#define _ENGINE_GRAPHICS_TYPEDATA_H_

#include "core\Types.h"

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    template <typename T>
    struct TypeData
    { };

    template <>
    struct TypeData<int>
    {
      static constexpr GLenum type = GL_INT;
      static constexpr GLenum component = GL_INT;
      static constexpr uint count = 1;
    };

    template <>
    struct TypeData<float>
    {
      static constexpr GLenum type = GL_FLOAT;
      static constexpr GLenum component = GL_FLOAT;
      static constexpr uint count = 1;
    };

    template <>
    struct TypeData<glm::vec2>
    {
      static constexpr GLenum type = GL_FLOAT_VEC2;
      static constexpr GLenum component = GL_FLOAT;
      static constexpr uint count = 2;
    };

    template <>
    struct TypeData<glm::vec3>
    {
      static constexpr GLenum type = GL_FLOAT_VEC3;
      static constexpr GLenum component = GL_FLOAT;
      static constexpr uint count = 3;
    };

    template <>
    struct TypeData<glm::vec4>
    {
      static constexpr GLenum type = GL_FLOAT_VEC4;
      static constexpr GLenum component = GL_FLOAT;
      static constexpr uint count = 4;
    };

    template <>
    struct TypeData<glm::mat3>
    {
      static constexpr GLenum type = GL_FLOAT_MAT3;
      static constexpr GLenum component = GL_FLOAT;
      static constexpr uint count = 9;
    };

    template <>
    struct TypeData<glm::mat4>
    {
      static constexpr GLenum type = GL_FLOAT_MAT4;
      static constexpr GLenum component = GL_FLOAT;
      static constexpr uint count = 16;
    };

    GLenum GetComponentType(GLenum _type);
    uint GetComponentCount(GLenum _type);
    uint GetTypeSize(GLenum _type);
  }
}

#endif //_ENGINE_GRAPHICS_TYPEDATA_H_