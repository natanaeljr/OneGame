#ifndef FIRSTGAME_EVENT_SCROLL_H_
#define FIRSTGAME_EVENT_SCROLL_H_

namespace firstgame::event {

struct ScrollEvent final {
    double xoffset;
    double yoffset;
};

}  // namespace firstgame::event

#endif  // FIRSTGAME_EVENT_SCROLL_H_
