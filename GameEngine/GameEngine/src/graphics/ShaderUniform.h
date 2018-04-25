#ifndef _ENGINE_GRAPHICS_SHADERUNIFORM_H_
#define _ENGINE_GRAPHICS_SHADERUNIFORM_H_

#include "core\Types.h"

#include "TypeData.h"

#include "Texture2D.h"
#include "TextureCube.h"

namespace engine
{
  namespace graphics
  {
    enum class SamplerType
    {
      SAMPLER_2D = GL_SAMPLER_2D,
      SAMPLER_CUBE = GL_SAMPLER_CUBE
    };

    GLenum SamplerTypeToOpenGL(SamplerType _sampler);
    SamplerType OpenGLToSamplerType(GLenum _sampler);

    struct ShaderAttribute
    {
      std::string name;
      Type type;
      GLint location;
    };

    struct ShaderUniform
    {
      std::string name;
      Type type;
      uint size;
      GLint location;
      int offset;
    };

    struct ShaderSampler
    {
      std::string name;
      SamplerType type;
      uint uniformIndex;
    };

    bool IsSupportedUniformType(GLenum _type);
    bool IsSamplerUniformType(GLenum _type);
  }
}

#endif //_ENGINE_GRAPHICS_SHADERUNIFORM_H_