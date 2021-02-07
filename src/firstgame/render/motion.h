#ifndef FIRSTGAME_RENDER_MOTION_H_
#define FIRSTGAME_RENDER_MOTION_H_

#include <glm/vec3.hpp>

namespace firstgame::render {

struct Motion final {
    glm::vec3 velocity{ 0.0f };
    glm::vec3 acceleration{ 0.0f };
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_MOTION_H_
