#ifndef _ENGINE_GRAPHICS_OPENGL_H_
#define _ENGINE_GRAPHICS_OPENGL_H_

#include "glew\glew.h"

namespace engine 
{ 
  namespace graphics 
  {
    namespace opengl
    {
      void ClearError();
      bool LogError(const char * _function, const char * _file, int _line);
    }
  } 
}

#if _DEBUG
#define GLCALL(x) \
    ::engine::graphics::opengl::ClearError(); \
    x; \
    ::engine::graphics::opengl::LogError(#x, __FILE__, __LINE__);
#else
  #define GLCALL(x) x;
#endif

#endif //_ENGINE_GRAPHICS_OPENGL_H_