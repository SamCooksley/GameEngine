#ifndef _ENGINE_GRAPHICS_SHADERTYPE_H_
#define _ENGINE_GRAPHICS_SHADERTYPE_H_

#include "core\Types.h"
#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    struct ShaderType
    {
    public:
      ShaderType() = delete;

      using Type = int8;

      static constexpr Type None = -1;
      static constexpr Type Vertex = 0;
      static constexpr Type Fragment = 1;
      static constexpr Type Geometry = 2;

      static constexpr Type Count = 3;

      static GLenum ToOpenGL(Type _type);
      static const std::string & ToString(Type _type);
      static Type FromString(const std::string & _string);
    };
  }
}

#endif //_ENGINE_GRAPHICS_SHADERTYPE_H_
