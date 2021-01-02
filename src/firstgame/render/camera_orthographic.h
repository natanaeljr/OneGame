#ifndef FIRSTGAME_RENDER_CAMERA_H_
#define FIRSTGAME_RENDER_CAMERA_H_

#include <glm/ext/vector_float3.hpp>
#include "view_projection.h"

namespace firstgame::render {

struct CameraOrthographic final {
    glm::vec3 position;
    float rotation;
    float aspect_ratio;
    float zoom_level;
};

void camera_orthographic_system_on_resize_canvas(int width, int height, CameraOrthographic& camera)
{
    camera.aspect_ratio = (float) width / (float) height;
}

void camera_orthographic_system_on_zoom(float offset, CameraOrthographic& camera)
{
    static constexpr float kZoomSpeed = 0.25f;
    camera.zoom_level = offset * kZoomSpeed;
    camera.zoom_level = std::max(camera.zoom_level, 0.25f);
}

ViewProjection camera_orthographic_view_projection(const CameraOrthographic& camera)
{
    return {
        .view =
            [&] {
                glm::mat4 transform =
                    glm::translate(glm::mat4(1.0f), camera.position) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(camera.rotation), glm::vec3(0, 0, 1));
                return glm::inverse(transform);
            }(),
        .projection = glm::ortho(-camera.aspect_ratio * camera.zoom_level,
                                 +camera.aspect_ratio * camera.zoom_level, -camera.zoom_level,
                                 +camera.zoom_level, -1.0f, +1.0f),
    };
}

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_H_
