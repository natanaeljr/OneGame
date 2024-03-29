#ifndef FIRSTGAME_RENDER_VIEW_PROJECTION_H_
#define FIRSTGAME_RENDER_VIEW_PROJECTION_H_

#include <glm/ext/matrix_float4x4.hpp>

namespace firstgame::render {

struct ViewProjection final {
    glm::mat4 view;
    glm::mat4 projection;
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_VIEW_PROJECTION_H_
