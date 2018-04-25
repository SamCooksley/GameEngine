#include "stdafx.h"

#include "ShaderUniform.h"

namespace engine
{
  namespace graphics
  {
    GLenum SamplerTypeToOpenGL(SamplerType _sampler)
    {
      return (GLenum)_sampler;
    }

    SamplerType OpenGLToSamplerType(GLenum _sampler)
    {
      switch (_sampler)
      {
        case GL_SAMPLER_2D: { return SamplerType::SAMPLER_2D; }
        case GL_SAMPLER_CUBE: { return SamplerType::SAMPLER_CUBE; }
        default: { assert(false); }
      }

      return SamplerType::SAMPLER_2D;
    }

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