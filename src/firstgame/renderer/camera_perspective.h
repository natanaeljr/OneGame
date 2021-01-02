#ifndef FIRSTGAME_RENDERER_CAMERA_PERSPECTIVE_H_
#define FIRSTGAME_RENDERER_CAMERA_PERSPECTIVE_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "view_projection.h"

namespace firstgame::renderer {

struct CameraPerspective final {
    float aspect_ratio;
    float fovy_degrees;
};

void camera_perspective_system_on_resize_canvas(int width, int height, CameraPerspective& camera)
{
    camera.aspect_ratio = (float) width / (float) height;
}

void camera_perspective_system_on_zoom(float offset, CameraPerspective& camera)
{
    static constexpr float kZoomSpeed = 2.0f;
    camera.fovy_degrees -= offset * kZoomSpeed;
    camera.fovy_degrees = std::min(camera.fovy_degrees, 45.0f);
    camera.fovy_degrees = std::max(camera.fovy_degrees, 1.0f);
}

ViewProjection camera_perspective_view_projection(const CameraPerspective& camera)
{
    return {
        .view =
            [] {
                auto view = glm::mat4(1.0f);
                view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
                return view;
            }(),
        .projection =
            glm::perspective(glm::radians(camera.fovy_degrees), camera.aspect_ratio, +1.0f, -1.0f),
    };
}

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_CAMERA_PERSPECTIVE_H_
