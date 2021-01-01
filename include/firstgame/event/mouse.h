#ifndef FIRSTGAME_EVENT_MOUSE_H_
#define FIRSTGAME_EVENT_MOUSE_H_

#include "firstgame/input/mouse.h"

namespace firstgame::event {

struct MouseEvent final {
    input::MouseButton button;
    input::MouseAction action;
};

}  // namespace firstgame::event

#endif  // FIRSTGAME_EVENT_MOUSE_H_
