#ifndef _ENGINE_GRAPHICS_WINDOW_H_
#define _ENGINE_GRAPHICS_WINDOW_H_

struct GLFWwindow;

namespace engine {
  
  namespace core { class glfw; }
  class Input;

  namespace graphics {

  class Window
  {
    friend class core::glfw;
    friend class Input;

  public:
    Window(const std::string & _title, int _width, int _height);
    ~Window();
    
    void Present();
    
    void Resize(int _width, int _height);
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
    int m_width;
    int m_height;

    bool m_fullscreen;
    bool m_vsync;
  };

} }

#endif //_ENGINE_GRAPHICS_WINDOW_H_