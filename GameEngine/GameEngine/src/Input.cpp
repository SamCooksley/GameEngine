#include "stdafx.h"

#include "Input.h"


#include "debug\Debug.h"
#include "Application.h"

#include "glfw\glfw3.h"

namespace engine {

  Input::KeyList Input::s_keys;
  Input::KeyList Input::s_keysDown;
  Input::KeyList Input::s_keysUp;

  Input::ButtonList Input::s_buttons;
  Input::ButtonList Input::s_buttonsDown;
  Input::ButtonList Input::s_buttonsUp;

  glm::vec2 Input::s_mousePosition;

  CursorMode Input::s_cursorMode = CursorMode::FREE;

  bool Input::getKey(KeyCode::Key _key)
  {
    for (size_t i = 0; i < s_keys.size(); ++i)
    {
      if (s_keys[i] == _key) { return true; }
    }
    return false;
  }

  bool Input::getKeyDown(KeyCode::Key _key)
  {
    for (size_t i = 0; i < s_keysDown.size(); ++i)
    {
      if (s_keysDown[i] == _key) { return true; }
    }
    return false;
  }

  bool Input::getKeyUp(KeyCode::Key _key)
  {
    for (size_t i = 0; i < s_keysUp.size(); ++i)
    {
      if (s_keysUp[i] == _key) { return true; }
    }
    return false;
  }

  bool Input::AnyKey()
  {
    return !s_keys.empty();
  }

  bool Input::AnyKeyDown()
  {
    return !s_keysDown.empty();
  }

  bool Input::AnyKeyUp()
  {
    return !s_keysUp.empty();
  }

  bool Input::getMouseButton(MouseButton::Button _button)
  {
    for (size_t i = 0; i < s_buttons.size(); ++i)
    {
      if (s_buttons[i] == _button) { return true; }
    }
    return false;
  }

  bool Input::getMouseButtonDown(MouseButton::Button _button)
  {
    for (size_t i = 0; i < s_buttonsDown.size(); ++i)
    {
      if (s_buttonsDown[i] == _button) { return true; }
    }
    return false;
  }

  bool Input::getMouseButtonUp(MouseButton::Button _button)
  {
    for (size_t i = 0; i < s_buttonsUp.size(); ++i)
    {
      if (s_buttonsUp[i] == _button) { return true; }
    }
    return false;
  }

  bool Input::AnyMouseButton()
  {
    return !s_buttons.empty();
  }

  bool Input::AnyMouseButtonDown()
  {
    return !s_buttonsDown.empty();
  }

  bool Input::AnyMouseButtonUp()
  {
    return !s_buttonsUp.empty();
  }

  const glm::vec2 & Input::getMousePosition()
  {
    return s_mousePosition;
  }

  void Input::PressKey(KeyCode::Key _key)
  {
    if (!getKey(_key))
    {
      s_keys.push_back(_key);
    }
    s_keysDown.push_back(_key);
  }

  void Input::ReleaseKey(KeyCode::Key _key)
  {
    for (size_t i = 0; i < s_keys.size();)
    {
      if (s_keys[i] == _key)
      {
        s_keys.erase(s_keys.begin() + i);
      }
      else { ++i; }
    }
    s_keysUp.push_back(_key);
  }

  void Input::PressButton(MouseButton::Button _button)
  {
    if (!getMouseButton(_button))
    {
      s_buttons.push_back(_button);
    }
    s_buttonsDown.push_back(_button);
  }

  void Input::ReleaseButton(MouseButton::Button _button)
  {
    for (size_t i = 0; i < s_buttons.size();)
    {
      if (s_buttons[i] == _button)
      {
        s_buttons.erase(s_buttons.begin() + i);
      }
      else { ++i; }
    }
    s_buttonsUp.push_back(_button);
  }

  void Input::setMousePosition(float _x, float _y)
  {
    s_mousePosition.x = _x;
    s_mousePosition.y = _y;
  }

  void Input::Reset()
  {
    s_keysDown.clear();
    s_keysUp.clear();

    s_buttonsDown.clear();
    s_buttonsUp.clear();
  }

  void Input::Init()
  {
    double x, y;
    glfwGetCursorPos(Application::s_context->window->m_window, &x, &y);
    s_mousePosition = glm::vec2(x, y);
  }

  void Input::setCursorMode(CursorMode _mode)
  {
    switch (_mode)
    {
      case CursorMode::FREE:
      {
        glfwSetInputMode(Application::s_context->window->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
      }
      case CursorMode::HIDDEN:
      {
        glfwSetInputMode(Application::s_context->window->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        break;
      }
      case engine::CursorMode::FIXED:
      {
        glfwSetInputMode(Application::s_context->window->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
      }
      default:
      {
        debug::LogWarning("Invalid cursor mode: " + std::to_string(static_cast<int>(_mode)));
        return;
      }
    }
    s_cursorMode = _mode;
  }

} // engine