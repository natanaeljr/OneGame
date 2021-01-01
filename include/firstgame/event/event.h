#ifndef FIRSTGAME_EVENT_EVENT_H_
#define FIRSTGAME_EVENT_EVENT_H_

#include <variant>

#include "key.h"
#include "cursor.h"
#include "mouse.h"
#include "scroll.h"
#include "joystick.h"
#include "window.h"

namespace firstgame::event {
// clang-format off

using Event = std::variant
<
    KeyEvent,
    CursorEvent,
    MouseEvent,
    ScrollEvent,
    JoystickEvent,
    WindowEvent
>;

// clang-format on
}  // namespace firstgame::event

#endif  // FIRSTGAME_EVENT_EVENT_H_
