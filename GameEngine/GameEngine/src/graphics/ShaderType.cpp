#include "stdafx.h"

#include "ShaderType.h"

namespace engine
{
  namespace graphics
  {
    GLenum ShaderType::ToOpenGL(ShaderType::Type _type)
    {
      switch (_type)
      {
        case ShaderType::Vertex:   { return GL_VERTEX_SHADER;   }
        case ShaderType::Fragment: { return GL_FRAGMENT_SHADER; }
        case ShaderType::Geometry: { return GL_GEOMETRY_SHADER; }
        default: { return 0; }
      }
    }

    static const std::array<std::string, ShaderType::Count + 1> s_shaderTypeString = {
      "vertex", "fragment", "geometry", "none"
    };

    const std::string & ShaderType::ToString(ShaderType::Type _type)
    {
      size_t index = static_cast<size_t>(_type);
      if (index >= s_shaderTypeString.size())
      {
        return s_shaderTypeString.back();
      }
      return s_shaderTypeString[index];
    }

    ShaderType::Type ShaderType::FromString(const std::string & _type)
    {
      for (ShaderType::Type i = 0; i < ShaderType::Count; ++i)
      {
        if (string::AreIEqual(_type, ShaderType::ToString(i)))
        {
          return i;
        }
      }

      return ShaderType::None;
    }
  }
}