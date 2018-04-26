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