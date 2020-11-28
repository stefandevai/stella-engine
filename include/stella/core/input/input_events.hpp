#pragma once

namespace stella::core
{

  enum class InputEvent
  {
    // Key down
    KEYDOWN_ARROW_UP,
    KEYDOWN_ARROW_RIGHT,
    KEYDOWN_ARROW_DOWN,
    KEYDOWN_ARROW_LEFT,
    KEYDOWN_W,
    KEYDOWN_D,
    KEYDOWN_S,
    KEYDOWN_A,
    KEYDOWN_SPACE,

    // Key up
    KEYUP_ARROW_UP,
    KEYUP_ARROW_RIGHT,
    KEYUP_ARROW_DOWN,
    KEYUP_ARROW_LEFT,
    KEYUP_W,
    KEYUP_D,
    KEYUP_S,
    KEYUP_A,
    KEYUP_SPACE,

    // Mouse
    MOUSE_LEFT_BUTTON_DOWN,
    MOUSE_LEFT_BUTTON_UP,
    MOUSE_RIGHT_BUTTON_DOWN,
    MOUSE_RIGHT_BUTTON_UP,
    MOUSE_MOVE,
  };

}
