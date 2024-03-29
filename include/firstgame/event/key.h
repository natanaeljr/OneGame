#ifndef FIRSTGAME_EVENT_KEY_H_
#define FIRSTGAME_EVENT_KEY_H_

#include "firstgame/input/key.h"

namespace firstgame::event {

struct KeyEvent final {
    input::Key key;
    input::KeyAction action;
};

}  // namespace firstgame::variant

#endif  // FIRSTGAME_EVENT_KEY_H_
