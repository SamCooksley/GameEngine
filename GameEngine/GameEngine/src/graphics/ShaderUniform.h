#ifndef _ENGINE_GRAPHICS_SHADERUNIFORM_H_
#define _ENGINE_GRAPHICS_SHADERUNIFORM_H_

#include "TypeData.h"

#include "Texture2D.h"
#include "TextureCube.h"

namespace engine {
namespace graphics {

  struct ShaderAttribute
  {
    String name;
    Type type;
    GLint location;
  };
  
  struct ShaderUniform
  {
    String name;
    Type type;
    uint size;
    GLint location;
    int offset;
  };
  
  struct ShaderSampler
  {
    String name;
    TextureType type;
    uint uniformIndex;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADERUNIFORM_H_