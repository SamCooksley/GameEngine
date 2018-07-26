#include "stdafx.h"

#include "opengl.h"

#include <sstream>

namespace engine {
namespace graphics {
namespace opengl {

  void ClearError()
  {
    while (glGetError() != GL_NO_ERROR);
  }
  
  bool LogError(const char * _function, const char * _file, int _line)
  {
    bool hadError = false;
    GLenum error = glGetError();
  
    while (error != GL_NO_ERROR)
    {
      debug::LogError(
        "OpenGL error " + std::to_string(error) + ": " + String((const char*)gluErrorString(error)) +
        " : Function: " + _function +
        " : File: " + _file +
        " : Line: " + std::to_string(_line)
      );
  
      hadError = true;
      error = glGetError();
    }
  
    return !hadError;
  }

} } } // engine::graphics::opengl