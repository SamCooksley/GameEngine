#ifndef _ENGINE_APPLICATION_H_
#define _ENGINE_APPLICATION_H_

#include "core/Context.h"

namespace engine
{
  class Application
  {
    friend class Input;
    friend class graphics::Window;
  public:
    Application() = delete;

    static void Init(int _argc, char** _argv);
    static void Loop();
    static void Exit();

    static void Quit();

  private:
    static void HandleEvents();
    static void Update();
    static void Render();

    static std::unique_ptr<core::Context> s_context;
  };
}

#endif //_ENGINE_APPLICATION_H_