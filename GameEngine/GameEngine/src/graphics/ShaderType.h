#ifndef _ENGINE_GRAPHICS_SHADERTYPE_H_
#define _ENGINE_GRAPHICS_SHADERTYPE_H_

#include "core\Types.h"
#include "opengl.h"

namespace engine {
namespace graphics {

  class ShaderType
  {
   public:
    ShaderType() = delete;
  
    enum Type
    {
      NONE     = -1,
      VERTEX   = 0,
      FRAGMENT,
      GEOMETRY,
  
      COUNT = 3
    };
  
    static GLenum ToOpenGL(Type _shader);
  
    static const String & ToString(Type _shader);
    static Type FromString(const String & _s);
  
   private:
    static const std::array<String, COUNT + 1> s_names;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADERTYPE_H_
