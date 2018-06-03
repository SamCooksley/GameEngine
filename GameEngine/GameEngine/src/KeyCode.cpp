#include "stdafx.h"

#include "KeyCode.h"

#include "glfw\glfw3.h"

namespace engine {

  const KeyCode::Key KeyCode::_0 = GLFW_KEY_0;
  const KeyCode::Key KeyCode::_1 = GLFW_KEY_1;
  const KeyCode::Key KeyCode::_2 = GLFW_KEY_2;
  const KeyCode::Key KeyCode::_3 = GLFW_KEY_3;
  const KeyCode::Key KeyCode::_4 = GLFW_KEY_4;
  const KeyCode::Key KeyCode::_5 = GLFW_KEY_5;
  const KeyCode::Key KeyCode::_6 = GLFW_KEY_6;
  const KeyCode::Key KeyCode::_7 = GLFW_KEY_7;
  const KeyCode::Key KeyCode::_8 = GLFW_KEY_8;
  const KeyCode::Key KeyCode::_9 = GLFW_KEY_9;

  const KeyCode::Key KeyCode::A = GLFW_KEY_A;
  const KeyCode::Key KeyCode::B = GLFW_KEY_B;
  const KeyCode::Key KeyCode::C = GLFW_KEY_C;
  const KeyCode::Key KeyCode::D = GLFW_KEY_D;
  const KeyCode::Key KeyCode::E = GLFW_KEY_E;
  const KeyCode::Key KeyCode::F = GLFW_KEY_F;
  const KeyCode::Key KeyCode::G = GLFW_KEY_G;
  const KeyCode::Key KeyCode::H = GLFW_KEY_H;
  const KeyCode::Key KeyCode::I = GLFW_KEY_I;
  const KeyCode::Key KeyCode::J = GLFW_KEY_J;
  const KeyCode::Key KeyCode::K = GLFW_KEY_K;
  const KeyCode::Key KeyCode::L = GLFW_KEY_L;
  const KeyCode::Key KeyCode::M = GLFW_KEY_M;
  const KeyCode::Key KeyCode::N = GLFW_KEY_N;
  const KeyCode::Key KeyCode::O = GLFW_KEY_O;
  const KeyCode::Key KeyCode::P = GLFW_KEY_P;
  const KeyCode::Key KeyCode::Q = GLFW_KEY_Q;
  const KeyCode::Key KeyCode::R = GLFW_KEY_R;
  const KeyCode::Key KeyCode::S = GLFW_KEY_S;
  const KeyCode::Key KeyCode::T = GLFW_KEY_T;
  const KeyCode::Key KeyCode::U = GLFW_KEY_U;
  const KeyCode::Key KeyCode::V = GLFW_KEY_V;
  const KeyCode::Key KeyCode::W = GLFW_KEY_W;
  const KeyCode::Key KeyCode::X = GLFW_KEY_X;
  const KeyCode::Key KeyCode::Y = GLFW_KEY_Y;
  const KeyCode::Key KeyCode::Z = GLFW_KEY_Z;

  const KeyCode::Key KeyCode::SPACE = GLFW_KEY_SPACE;
  const KeyCode::Key KeyCode::APOSTROPHE = GLFW_KEY_APOSTROPHE;
  const KeyCode::Key KeyCode::COMMA = GLFW_KEY_COMMA;
  const KeyCode::Key KeyCode::MINUS = GLFW_KEY_MINUS;
  const KeyCode::Key KeyCode::PERIOD = GLFW_KEY_PERIOD;
  const KeyCode::Key KeyCode::SLASH = GLFW_KEY_SLASH;
  const KeyCode::Key KeyCode::SEMICOLON = GLFW_KEY_SEMICOLON;
  const KeyCode::Key KeyCode::EQUAL = GLFW_KEY_EQUAL;
  const KeyCode::Key KeyCode::LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
  const KeyCode::Key KeyCode::RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
  const KeyCode::Key KeyCode::BACKSLASH = GLFW_KEY_BACKSLASH;
  const KeyCode::Key KeyCode::GRAVE = GLFW_KEY_GRAVE_ACCENT;

  const KeyCode::Key KeyCode::ESCAPE = GLFW_KEY_ESCAPE;
  const KeyCode::Key KeyCode::ENTER = GLFW_KEY_ENTER;
  const KeyCode::Key KeyCode::TAB = GLFW_KEY_TAB;
  const KeyCode::Key KeyCode::BACKSPACE = GLFW_KEY_BACKSPACE;

  const KeyCode::Key KeyCode::INSERT = GLFW_KEY_INSERT;
  const KeyCode::Key KeyCode::DELETE = GLFW_KEY_DELETE;
  const KeyCode::Key KeyCode::PAGE_UP = GLFW_KEY_PAGE_UP;
  const KeyCode::Key KeyCode::PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
  const KeyCode::Key KeyCode::HOME = GLFW_KEY_HOME;
  const KeyCode::Key KeyCode::END = GLFW_KEY_END;

  const KeyCode::Key KeyCode::CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
  const KeyCode::Key KeyCode::SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
  const KeyCode::Key KeyCode::NUM_LOCK = GLFW_KEY_NUM_LOCK;
  const KeyCode::Key KeyCode::PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
  const KeyCode::Key KeyCode::PAUSE = GLFW_KEY_PAUSE;

  const KeyCode::Key KeyCode::F1 = GLFW_KEY_F1;
  const KeyCode::Key KeyCode::F2 = GLFW_KEY_F2;
  const KeyCode::Key KeyCode::F3 = GLFW_KEY_F3;
  const KeyCode::Key KeyCode::F4 = GLFW_KEY_F4;
  const KeyCode::Key KeyCode::F5 = GLFW_KEY_F5;
  const KeyCode::Key KeyCode::F6 = GLFW_KEY_F6;
  const KeyCode::Key KeyCode::F7 = GLFW_KEY_F7;
  const KeyCode::Key KeyCode::F8 = GLFW_KEY_F8;
  const KeyCode::Key KeyCode::F9 = GLFW_KEY_F9;
  const KeyCode::Key KeyCode::F10 = GLFW_KEY_F10;
  const KeyCode::Key KeyCode::F11 = GLFW_KEY_F11;
  const KeyCode::Key KeyCode::F12 = GLFW_KEY_F12;

  const KeyCode::Key KeyCode::RIGHT = GLFW_KEY_RIGHT;
  const KeyCode::Key KeyCode::LEFT = GLFW_KEY_LEFT;
  const KeyCode::Key KeyCode::DOWN = GLFW_KEY_DOWN;
  const KeyCode::Key KeyCode::UP = GLFW_KEY_UP;

  const KeyCode::Key KeyCode::NUMPAD_0 = GLFW_KEY_KP_0;
  const KeyCode::Key KeyCode::NUMPAD_1 = GLFW_KEY_KP_1;
  const KeyCode::Key KeyCode::NUMPAD_2 = GLFW_KEY_KP_2;
  const KeyCode::Key KeyCode::NUMPAD_3 = GLFW_KEY_KP_3;
  const KeyCode::Key KeyCode::NUMPAD_4 = GLFW_KEY_KP_4;
  const KeyCode::Key KeyCode::NUMPAD_5 = GLFW_KEY_KP_5;
  const KeyCode::Key KeyCode::NUMPAD_6 = GLFW_KEY_KP_6;
  const KeyCode::Key KeyCode::NUMPAD_7 = GLFW_KEY_KP_7;
  const KeyCode::Key KeyCode::NUMPAD_8 = GLFW_KEY_KP_8;
  const KeyCode::Key KeyCode::NUMPAD_9 = GLFW_KEY_KP_9;
  const KeyCode::Key KeyCode::NUMPAD_DECIMAL = GLFW_KEY_KP_DECIMAL;
  const KeyCode::Key KeyCode::NUMPAD_DIVIDE = GLFW_KEY_KP_DIVIDE;
  const KeyCode::Key KeyCode::NUMPAD_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
  const KeyCode::Key KeyCode::NUMPAD_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
  const KeyCode::Key KeyCode::NUMPAD_ADD = GLFW_KEY_KP_ADD;
  const KeyCode::Key KeyCode::NUMPAD_ENTER = GLFW_KEY_KP_ENTER;
  const KeyCode::Key KeyCode::NUMPAD_EQUAL = GLFW_KEY_KP_EQUAL;

  const KeyCode::Key KeyCode::LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
  const KeyCode::Key KeyCode::LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
  const KeyCode::Key KeyCode::LEFT_ALT = GLFW_KEY_LEFT_ALT;
  const KeyCode::Key KeyCode::LEFT_WINDOWS = GLFW_KEY_LEFT_SUPER;
  const KeyCode::Key KeyCode::RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
  const KeyCode::Key KeyCode::RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
  const KeyCode::Key KeyCode::RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
  const KeyCode::Key KeyCode::RIGHT_WIDNOWS = GLFW_KEY_RIGHT_SUPER;
  const KeyCode::Key KeyCode::MENU = GLFW_KEY_MENU;

  const MouseButton::Button MouseButton::LEFT = GLFW_MOUSE_BUTTON_LEFT;
  const MouseButton::Button MouseButton::RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
  const MouseButton::Button MouseButton::MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
  const MouseButton::Button MouseButton::_1 = GLFW_MOUSE_BUTTON_4;
  const MouseButton::Button MouseButton::_2 = GLFW_MOUSE_BUTTON_5;

} // engine