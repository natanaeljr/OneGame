#ifndef FIRSTGAME_INPUT_MOUSE_H_
#define FIRSTGAME_INPUT_MOUSE_H_

namespace firstgame::input {

enum class MouseAction {
    Release = 0,
    Press = 1,
};

enum class MouseButton {
    Unknown = -1,
    Left = 0,
    Middle = 1,
    Right = 2,
};

}  // namespace firstgame::input

#endif  // FIRSTGAME_INPUT_MOUSE_H_
