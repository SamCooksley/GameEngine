#include "stdafx.h"

#include "ShaderType.h"
#include "opengl.h"

namespace engine {
namespace graphics {

  const std::array<String, ShaderType::COUNT + 1> ShaderType::s_names = {
    "vertex", "fragment", "geometry", "none"
  };
  
  GLenum ShaderType::ToOpenGL(Type _shader)
  {
    switch (_shader)
    {
      case VERTEX:   { return GL_VERTEX_SHADER;   }
      case FRAGMENT: { return GL_FRAGMENT_SHADER; }
      case GEOMETRY: { return GL_GEOMETRY_SHADER; }
      default:
      {
        throw std::invalid_argument("ShaderType.ToOpenGL");
      }
    }
  }
  
  const String & ShaderType::ToString(Type _shader)
  {
    if (_shader == NONE)
    {
      return s_names.back();
    }
  
    if (_shader < 0 || _shader >= COUNT)
    {
      throw std::invalid_argument("ShaderType.ToString");
    }
  
    return s_names[_shader];
  }
  
  ShaderType::Type ShaderType::FromString(const String & _s)
  {
    for (size_t i = 0u; i < s_names.size(); ++i)
    {
      if (string::AreIEqual(s_names[i], _s))
      {
        return static_cast<Type>(i);
      }
    }
  
    throw std::invalid_argument("ShaderType.FromString");
  }

} } // engine::graphics