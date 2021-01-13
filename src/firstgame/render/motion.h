#ifndef FIRSTGAME_RENDER_MOTION_H_
#define FIRSTGAME_RENDER_MOTION_H_

#include <glm/vec3.hpp>

namespace firstgame::render {

struct Motion final {
    glm::vec3 velocity;
    glm::vec3 acceleration;  // force/mass
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_MOTION_H_
