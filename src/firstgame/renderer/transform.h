#ifndef FIRSTGAME_TRANSFORM_H_
#define FIRSTGAME_TRANSFORM_H_

#include <glm/vec3.hpp>

namespace firstgame::renderer {

struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_TRANSFORM_H_
