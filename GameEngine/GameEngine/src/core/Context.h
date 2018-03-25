#ifndef _ENGINE_CORE_CONTEXT_H_
#define _ENGINE_CORE_CONTEXT_H_

#include "glfw.h"

#include "graphics\Window.h"

namespace engine { namespace core {

  enum class EngineState
  {
    None,
    Init,
    Running,
    Exit
  };

  struct Context
  {
  public:
    EngineState state;

    std::vector<std::string> args;

    std::unique_ptr<glfw> glfwContext;

    std::unique_ptr<graphics::Window> window;
  };

} }

#endif //_ENGINE_CORE_CONTEXT_H_