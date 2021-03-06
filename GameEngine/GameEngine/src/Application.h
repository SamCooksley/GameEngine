#ifndef _ENGINE_APPLICATION_H_
#define _ENGINE_APPLICATION_H_

#include "core/Context.h"

namespace engine {

  class Application
  {
    friend class Input;
    friend class Resources;
    friend class Time;
    friend class graphics::Window;
    friend class GameObject;
    friend class Scene;
    friend class Screen;
    friend class Graphics;
    friend class Camera;
    friend class Light;

   public:
    Application() = delete;

    static void Init(int _argc, char** _argv);
    static void Loop();
    static void Exit();

    static void Quit();

   private:
    static void Frame();

    static void HandleEvents();
    static void Update();
    static void Render();

    static void ChangeScene();

   private:
    static std::unique_ptr<Context> s_context;
  };

} // engine

#endif //_ENGINE_APPLICATION_H_