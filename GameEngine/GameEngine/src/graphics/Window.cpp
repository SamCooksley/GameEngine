#include "stdafx.h"

#include "Window.h"

#include "glfw\glfw3.h"

#include "core\glfw.h"
#include "Application.h"

namespace engine { namespace graphics {

  Window::Window(const std::string & _title, int _width, int _height)
  {
    m_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
      throw std::runtime_error("Failed to create window.");
    }

    MakeCurrent();
   
    glfwSwapInterval(0);

    m_width = _width;
    m_height = _height;

    m_title = _title;

    m_fullscreen = false;
    m_vsync = false;

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, core::glfw::key_callback);
    glfwSetMouseButtonCallback(m_window, core::glfw::mouse_button_callback);
    glfwSetCursorPosCallback(m_window, core::glfw::mouse_position_callback);

    glfwSetWindowSizeCallback(m_window, core::glfw::window_size_callback);
    glfwSetWindowCloseCallback(m_window, core::glfw::window_close_callback);
  }

  Window::~Window()
  {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
    m_width = m_height = 0;
  }

  void Window::Present()
  {
    glfwSwapBuffers(m_window);
  }

  void Window::Resize(int _width, int _height)
  {
    glfwSetWindowSize(m_window, _width, _height);
    m_width = _width;
    m_height = _height;
  }

  void Window::setTitle(const std::string & _title)
  {
    glfwSetWindowTitle(m_window, _title.c_str());
    m_title = _title;
  }

  void Window::setFullscreen(bool _fullscreen)
  {
    if (m_fullscreen != _fullscreen)
    {
      glfwSetWindowMonitor(
        m_window,
        _fullscreen ? glfwGetPrimaryMonitor() : nullptr, 
        0, 0, 
        m_width, m_height,
        GLFW_DONT_CARE
      );
      m_fullscreen = _fullscreen;
    }
  }

  void Window::setVsync(bool _vsync)
  {
    if (m_vsync != _vsync)
    {
      glfwSwapInterval(_vsync ? 1 : 0);
      m_vsync = _vsync;
    }
  }

  void Window::MakeCurrent()
  {
    glfwMakeContextCurrent(m_window);
  }

  void Window::Close()
  {
    if (Application::s_context->window.get() == this)
    {
      Application::Quit();
    }
  }

  int Window::getWidth() const
  {
    return m_width;
  }

  int Window::getHeight() const
  {
    return m_height;
  }

} }