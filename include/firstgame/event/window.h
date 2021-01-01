#ifndef FIRSTGAME_EVENT_WINDOW_H_
#define FIRSTGAME_EVENT_WINDOW_H_

#include <variant>

namespace firstgame::event {

struct WindowEvent final {
    struct Resize {
        int width;
        int height;
    };
    enum class Focus {
        Gained,
        Lost,
    };
    enum class Imize {
        Max,
        Min,
    };

    std::variant<Resize, Focus, Imize> variant;
};

}  // namespace firstgame::event

#endif  // FIRSTGAME_EVENT_WINDOW_H_
