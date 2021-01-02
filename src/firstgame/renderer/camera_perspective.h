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

    void Resize(int width, int height) { aspect_ratio = (float) width / (float) height; }
    void Zoom(float offset)
    {
        static constexpr float kZoomSpeed = 2.0f;
        fovy_degrees -= offset * kZoomSpeed;
        fovy_degrees = std::min(fovy_degrees, 45.0f);
        fovy_degrees = std::max(fovy_degrees, 1.0f);
    }
    [[nodiscard]] ViewProjection ToViewProjection() const
    {
        return {
            .view =
                [] {
                    auto view = glm::mat4(1.0f);
                    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
                    return view;
                }(),
            .projection = glm::perspective(glm::radians(fovy_degrees), aspect_ratio, +1.0f, -1.0f),
        };
    }
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_CAMERA_PERSPECTIVE_H_
