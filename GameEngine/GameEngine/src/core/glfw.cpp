#include "stdafx.h"

#include "glfw.h"

#include "glfw\glfw3.h"

#include "debug\Debug.h"
#include "Input.h"

namespace engine { namespace core { 

  void glfw::key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
  {
    switch (_action)
    {
      case GLFW_PRESS:   { Input::PressKey(_key);   break; }
      case GLFW_RELEASE: { Input::ReleaseKey(_key); break; }
    }
  }

  void glfw::mouse_button_callback(GLFWwindow* _window, int _button, int _action, int _mods)
  {
    switch (_action)
    {
      case GLFW_PRESS:   { Input::PressButton(_button);   break; }
      case GLFW_RELEASE: { Input::ReleaseButton(_button); break; }
    }
  }

  void glfw::mouse_position_callback(GLFWwindow* _window, double _x, double _y)
  {
    Input::setMousePosition(static_cast<float>(_x), static_cast<float>(_y));
  }

  void glfw::window_size_callback(GLFWwindow* _window, int _width, int _height)
  {
    graphics::Window* window = static_cast<graphics::Window*>(glfwGetWindowUserPointer(_window));

    window->m_width = _width;
    window->m_height = _height;
  }

  void glfw::window_close_callback(GLFWwindow* _window)
  {
    graphics::Window* window = static_cast<graphics::Window*>(glfwGetWindowUserPointer(_window));
    window->Close();
  }

  void glfw::error_callback(int _error, const char* _description)
  {
    debug::LogError("GLFW error: " + std::to_string(_error) + " - " + _description);
  }

  bool glfw::s_init = false;

  glfw::glfw()
  {
    if (s_init)
    {
      throw std::runtime_error("glfw already initialised.");
    }

    if (glfwInit() == GLFW_FALSE)
    {
      throw std::runtime_error("Failed to initialise glfw.");
    }
    s_init = true;
    glfwSetErrorCallback(error_callback);
  }

  glfw::~glfw()
  {
    glfwTerminate();
    s_init = false;
  }

  void glfw::PollEvents()
  {
    glfwPollEvents();
  }

} }