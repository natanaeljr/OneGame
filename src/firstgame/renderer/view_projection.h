#ifndef FIRSTGAME_RENDERER_VIEW_PROJECTION_H_
#define FIRSTGAME_RENDERER_VIEW_PROJECTION_H_

#include <glm/ext/matrix_float4x4.hpp>

namespace firstgame::renderer {

struct ViewProjection final {
    glm::mat4 view;
    glm::mat4 projection;
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_VIEW_PROJECTION_H_
