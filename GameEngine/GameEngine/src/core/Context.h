#ifndef _ENGINE_CORE_CONTEXT_H_
#define _ENGINE_CORE_CONTEXT_H_

#include "glfw.h"

#include "Scene.h"

#include "utilities\Timer.h"

#include "Asset.h"

#include "Camera.h"
#include "Light.h"

#include "graphics\Graphics.h"
#include "graphics\Window.h"

namespace engine {

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
  
    std::vector<String> args;
  
    std::map<String, std::shared_ptr<Asset>> resources;
  
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

    std::unique_ptr<graphics::BaseRenderer> renderer;
  
    std::shared_ptr<Camera> camera;
  };
  
  void Destroy(std::unique_ptr<Context> & _context);

} // engine

#endif //_ENGINE_CORE_CONTEXT_H_