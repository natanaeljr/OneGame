#ifndef FIRSTGAME_RENDER_CAMERA_H_
#define FIRSTGAME_RENDER_CAMERA_H_

#include <glm/ext/vector_float3.hpp>
#include "view_projection.h"
#include "../util/size.h"

namespace firstgame::render {

struct CameraOrthographic final {
    explicit CameraOrthographic(util::SizeT<float> size)
    {
        Resize(size);
        Zoom(0.0f);
    }

    void Resize(util::SizeT<float> size)
    {
        aspect_ratio = size.width / size.height;
        matrix.projection = glm::ortho(-aspect_ratio * zoom_level, +aspect_ratio * zoom_level,
                                       -zoom_level, +zoom_level, -1.0f, +1.0f);
    }

    void Zoom(float offset)
    {
        static constexpr float kZoomSpeed = 0.25f;
        zoom_level = offset * kZoomSpeed;
        zoom_level = std::max(zoom_level, 0.25f);
        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
        matrix.view = glm::inverse(transform);
    }

    [[nodiscard]] const ViewProjection& Matrix() const { return matrix; }

   private:
    glm::vec3 position{ 0.0f };
    float rotation{ 0.0f };
    float aspect_ratio{ 1.0f };
    float zoom_level{ 0.0f };
    ViewProjection matrix{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_H_
