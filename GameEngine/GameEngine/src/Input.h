#ifndef _ENGINE_INPUT_H_
#define _ENGINE_INPUT_H_

#include "KeyCode.h"

namespace engine {

  class glfw;

  enum class CursorMode
  {
    FREE,
    HIDDEN,
    FIXED
  };

  class Input
  {
    friend class Application;
    friend class glfw;

   private:
    using KeyList = std::vector<KeyCode::Key>;
    using ButtonList = std::vector<MouseButton::Button>;

   public:
    Input() = delete;

    static bool getKey(KeyCode::Key _key);    
    static bool getKeyDown(KeyCode::Key _key);
    static bool getKeyUp(KeyCode::Key _key);  

    static bool AnyKey();     
    static bool AnyKeyDown(); 
    static bool AnyKeyUp();   

    static bool getMouseButton(MouseButton::Button _button);     
    static bool getMouseButtonDown(MouseButton::Button _button); 
    static bool getMouseButtonUp(MouseButton::Button _button);   

    static bool AnyMouseButton();     
    static bool AnyMouseButtonDown(); 
    static bool AnyMouseButtonUp();   

    static const glm::vec2 & getMousePosition();

    static void setCursorMode(CursorMode _mode);

   private:
    static void PressKey(KeyCode::Key _key);
    static void ReleaseKey(KeyCode::Key _key);

    static void PressButton(MouseButton::Button _button); 
    static void ReleaseButton(MouseButton::Button _button); 

    static void setMousePosition(float _x, float _y);

    static void Reset();

    static void Init();
 
   private:
    static KeyList s_keys;
    static KeyList s_keysDown;
    static KeyList s_keysUp;

    static ButtonList s_buttons;
    static ButtonList s_buttonsDown;
    static ButtonList s_buttonsUp;   

    static glm::vec2 s_mousePosition;

    static CursorMode s_cursorMode;
  };

} // engine

#endif //_ENGINE_INPUT_H_