#ifndef _ENGINE_KEYCODE_H_
#define _ENGINE_KEYCODE_H_

#include "core\Types.h"

namespace engine {

  class KeyCode
  {
   public:
    using Key = uint;

    static const Key
      _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,

      A, B, C, D, E, F, G, H, I, J, K, L, M,
      N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

      SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
      SEMICOLON, EQUAL, LEFT_BRACKET, RIGHT_BRACKET,
      BACKSLASH, GRAVE,

      ESCAPE, ENTER, TAB, BACKSPACE,
      INSERT, DELETE, PAGE_UP, PAGE_DOWN, HOME, END,

      CAPS_LOCK, SCROLL_LOCK, NUM_LOCK,
      PRINT_SCREEN, PAUSE,

      F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

      RIGHT, LEFT, DOWN, UP,

      NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4,
      NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
      NUMPAD_DECIMAL, NUMPAD_DIVIDE, NUMPAD_MULTIPLY,
      NUMPAD_SUBTRACT, NUMPAD_ADD, NUMPAD_ENTER, NUMPAD_EQUAL,

      LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_WINDOWS,
      RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_WIDNOWS,
      MENU;
  };

  class MouseButton
  {
   public:
    using Button = uint;

    static const Button
      LEFT, RIGHT, MIDDLE,
      _1, _2;
  };

} // engine

#endif //_ENGINE_KEYCODE_H_