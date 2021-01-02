#ifndef FIRSTGAME_RENDER_TRANSFORM_H_
#define FIRSTGAME_RENDER_TRANSFORM_H_

#include <glm/vec3.hpp>

namespace firstgame::render {

struct Transform {
    glm::vec3 position;
    glm::vec3 scale;
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_TRANSFORM_H_
