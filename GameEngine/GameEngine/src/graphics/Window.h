#ifndef _ENGINE_GRAPHICS_WINDOW_H_
#define _ENGINE_GRAPHICS_WINDOW_H_

#include "core\Types.h"

struct GLFWwindow;

namespace engine {

class glfw;
class Input;

namespace graphics {

  class Window
  {
    friend class glfw;
    friend class Input;

   public:
    Window(const String & _title, uint _width, uint _height);
    ~Window();

    void Present();

    void Resize(uint _width, uint _height);
    void setTitle(const String & _title);

    void setFullscreen(bool _fullscreen);
    void setVsync(bool _vsync);

    void MakeCurrent();

    int getWidth() const;
    int getHeight() const;

   private:
    void Close();

   private:
    GLFWwindow * m_window;

    String m_title;
    uint m_width, m_height;

    bool m_fullscreen, m_vsync;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_WINDOW_H_