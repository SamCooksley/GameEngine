#ifndef _ENGINE_GRAPHICS_WINDOW_H_
#define _ENGINE_GRAPHICS_WINDOW_H_

#include "core\Types.h"

struct GLFWwindow;

namespace engine
{
  namespace core { class glfw; }
  class Input;

  namespace graphics
  {
    class Window
    {
      friend class core::glfw;
      friend class Input;

    public:
      Window(const std::string & _title, uint _width, uint _height);
      ~Window();

      void Present();

      void Resize(uint _width, uint _height);
      void setTitle(const std::string & _title);

      void setFullscreen(bool _fullscreen);
      void setVsync(bool _vsync);

      void MakeCurrent();

      int getWidth() const;
      int getHeight() const;

    private:
      void Close();

      GLFWwindow * m_window;

      std::string m_title;
      uint m_width, m_height;

      bool m_fullscreen, m_vsync;
    };
  }
}

#endif //_ENGINE_GRAPHICS_WINDOW_H_