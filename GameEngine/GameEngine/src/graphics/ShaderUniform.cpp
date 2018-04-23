#include "stdafx.h"

#include "ShaderUniform.h"

namespace engine
{
  namespace graphics
  {
    bool IsSupportedUniformType(GLenum _type)
    {
      switch (_type)
      {
      case GL_INT:          { return true; }
      case GL_FLOAT:        { return true; }
      case GL_FLOAT_VEC2:   { return true; }
      case GL_FLOAT_VEC3:   { return true; }
      case GL_FLOAT_VEC4:   { return true; }
      case GL_FLOAT_MAT3:   { return true; }
      case GL_FLOAT_MAT4:   { return true; }
      case GL_SAMPLER_2D:   { return true; }
      case GL_SAMPLER_CUBE: { return true; }
      default: { return false; }
      }
    }

    bool IsSamplerUniformType(GLenum _type)
    {
      switch (_type)
      {
      case GL_SAMPLER_2D:   { return true; }
      case GL_SAMPLER_CUBE: { return true; }
      default: { return false; }
      }
    }
  }
}