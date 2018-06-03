#ifndef _ENGINE_CORE_CONTEXT_H_
#define _ENGINE_CORE_CONTEXT_H_

#include "graphics\Graphics.h"


#include "graphics\Window.h"

#include "glfw.h"
#include "Scene.h"

#include "utilities\Timer.h"

#include "Camera.h"

#include "Asset.h"

namespace engine
{
  namespace core
  {
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

      std::map<std::string, std::shared_ptr<Asset>> resources;

      std::unique_ptr<glfw> glfwContext;

      utilities::Timer frameTime;
      float targetFrameTime;
      int maxUpdatesPerFrame;
      float totalDeltaTime;
      float deltaTime;

      std::unique_ptr<graphics::Window> window;

      std::shared_ptr<Scene> scene;
      std::shared_ptr<Scene> nextScene;

      std::unique_ptr<graphics::Context> graphics;

      std::vector<std::weak_ptr<Camera>> cameras;
    };
  }

  void Destroy(std::unique_ptr<core::Context> & _context);
}

#endif //_ENGINE_CORE_CONTEXT_H_