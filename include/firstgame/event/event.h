#ifndef FIRSTGAME_EVENT_EVENT_H_
#define FIRSTGAME_EVENT_EVENT_H_

#include <variant>
#include "key.h"
#include "mouse.h"
#include "joystick.h"
#include "window.h"

namespace firstgame::event {

struct Event {
    std::variant<KeyEvent, MouseEvent, JoystickEvent, WindowEvent> event;
};

}  // namespace firstgame::event

#endif  // FIRSTGAME_EVENT_EVENT_H_
