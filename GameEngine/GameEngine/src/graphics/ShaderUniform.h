#ifndef _ENGINE_GRAPHICS_SHADERUNIFORM_H_
#define _ENGINE_GRAPHICS_SHADERUNIFORM_H_

#include "core\Types.h"

namespace engine { namespace graphics {

  enum class Type
  {
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4
  };

  struct ShaderUniform
  {
    std::string name;
    Type type;
    int location;
    uint size;
    uint offset;
  };

  struct ShaderTexture
  {
    std::string name;
    int textureUnit;
    int location;
  };

  struct ShaderAttribute
  {
    std::string name;
    uint type;
    uint count;
    bool normalized;
    int location;
  };

} }

#endif //_ENGINE_GRAPHICS_SHADERUNIFORM_H_