#ifndef FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_
#define FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "view_projection.h"
#include "../util/size.h"

namespace firstgame::render {

struct CameraPerspective final {
    explicit CameraPerspective(util::SizeT<float> size)
    {
        Resize(size);
        Zoom(0.0f);
    }

    void Resize(util::SizeT<float> size)
    {
        aspect_ratio = size.width / size.height;
        matrix.projection =
            glm::perspective(glm::radians(fovy_degrees), aspect_ratio, +1.0f, -1.0f);
    }

    void Zoom(float offset)
    {
        static constexpr float kZoomSpeed = 2.0f;
        fovy_degrees -= offset * kZoomSpeed;
        fovy_degrees = std::min(fovy_degrees, 45.0f);
        fovy_degrees = std::max(fovy_degrees, 1.0f);
        matrix.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        matrix.projection =
            glm::perspective(glm::radians(fovy_degrees), aspect_ratio, +1.0f, -1.0f);
    }

    [[nodiscard]] const ViewProjection& Matrix() const { return matrix; }

   private:
    float aspect_ratio{ 1.0f };
    float fovy_degrees{ 45.0f };
    ViewProjection matrix{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_
