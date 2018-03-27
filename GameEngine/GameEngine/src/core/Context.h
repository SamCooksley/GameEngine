#ifndef _ENGINE_CORE_CONTEXT_H_
#define _ENGINE_CORE_CONTEXT_H_

#include "glfw.h"

#include "graphics\Window.h"

#include "Scene.h"

#include "utilities\Timer.h"

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

    utilities::Timer frameTime;
    float targetFrameTime;
    int maxUpdatesPerFrame;
    float totalDeltaTime;
    float deltaTime;

    std::unique_ptr<graphics::Window> window;

    std::shared_ptr<Scene> scene;
    std::shared_ptr<Scene> nextScene;
  };

} }

#endif //_ENGINE_CORE_CONTEXT_H_