#ifndef _ENGINE_CORE_GLFW_H_
#define _ENGINE_CORE_GLFW_H_

#include "graphics\Window.h"

namespace engine { namespace core {

  class glfw
  {
    friend class Application;
    friend class graphics::Window;

  private:
    static void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
    
    static void mouse_button_callback(GLFWwindow* _window, int _button, int _action, int _mods);
    static void mouse_position_callback(GLFWwindow* _window, double _x, double _y);
    
    static void window_size_callback(GLFWwindow* _window, int _width, int _height);
    static void window_close_callback(GLFWwindow* _window);
    
    static void error_callback(int _error, const char* _description);

    static bool s_init;

  public:
    glfw();
    ~glfw();

    void PollEvents();
  };

} }

#endif //_ENGINE_CORE_GLFW_H_