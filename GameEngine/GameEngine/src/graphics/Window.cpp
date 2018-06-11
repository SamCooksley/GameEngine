#include "stdafx.h"

#include "Window.h"

#include "core\glfw.h"
#include "Application.h"

#include "glfw\glfw3.h"

namespace engine {
namespace graphics {

  Window::Window(const String & _title, uint _width, uint _height)
  {
    m_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
  
    if (m_window == nullptr)
    {
      throw std::runtime_error("Window Error: failed to create window.");
    }
  
    MakeCurrent();
  
    glfwSwapInterval(0);
  
    m_width = _width;
    m_height = _height;
  
    m_title = _title;
  
    m_fullscreen = false;
    m_vsync = false;
  
    glfwSetWindowUserPointer(m_window, this);
  
    glfwSetKeyCallback(m_window, glfw::key_callback);
    glfwSetMouseButtonCallback(m_window, glfw::mouse_button_callback);
    glfwSetCursorPosCallback(m_window, glfw::mouse_position_callback);
  
    glfwSetWindowSizeCallback(m_window, glfw::window_size_callback);
    glfwSetWindowCloseCallback(m_window, glfw::window_close_callback);

    glfwSetFramebufferSizeCallback(m_window, glfw::framebuffer_size_callback);
  }
  
  Window::~Window()
  {
    glfwDestroyWindow(m_window);
  }
  
  void Window::Present()
  {
    glfwSwapBuffers(m_window);
  }
  
  void Window::Resize(uint _width, uint _height)
  {
    glfwSetWindowSize(m_window, _width, _height);
    m_width = _width;
    m_height = _height;
  }
  
  void Window::setTitle(const String & _title)
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

} } // engine::graphics