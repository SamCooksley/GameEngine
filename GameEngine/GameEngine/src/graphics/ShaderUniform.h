#ifndef _ENGINE_GRAPHICS_SHADERUNIFORM_H_
#define _ENGINE_GRAPHICS_SHADERUNIFORM_H_

#include "core\Types.h"

#include "TypeData.h"

namespace engine
{
  namespace graphics
  {

    struct ShaderUniform
    {
      std::string name;
      GLenum type;
      GLint location;
      uint size;
      uint offset;
    };

    struct ShaderTexture
    {
      std::string name;
      uint textureUnit;
      GLint location;
    };

    struct ShaderAttribute
    {
      std::string name;
      GLenum type;
      uint count;
      bool normalized;
      GLint location;
    };

  }
}

#endif //_ENGINE_GRAPHICS_SHADERUNIFORM_H_