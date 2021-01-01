#ifndef FIRSTGAME_RENDERER_CAMERA_H_
#define FIRSTGAME_RENDERER_CAMERA_H_

#include <glm/mat4x4.hpp>

namespace firstgame::renderer {

struct Camera {
    glm::mat4 view;
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_CAMERA_H_
