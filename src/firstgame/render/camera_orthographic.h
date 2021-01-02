#ifndef FIRSTGAME_RENDER_CAMERA_H_
#define FIRSTGAME_RENDER_CAMERA_H_

#include <glm/ext/vector_float3.hpp>
#include "view_projection.h"
#include "../util/size.h"

namespace firstgame::render {

class CameraOrthographic final {
   public:
    explicit CameraOrthographic(util::SizeT<float> size)
    {
        Resize(size);
        Zoom(0.0f);
    }

    void Resize(util::SizeT<float> size)
    {
        aspect_ratio_ = size.width / size.height;
        matrix_.projection = glm::ortho(-aspect_ratio_ * zoom_level_, +aspect_ratio_ * zoom_level_,
                                        -zoom_level_, +zoom_level_, -1.0f, +1.0f);
    }

    void Zoom(float offset)
    {
        static constexpr float kZoomSpeed = 0.25f;
        zoom_level_ = offset * kZoomSpeed;
        zoom_level_ = std::max(zoom_level_, 0.25f);
        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position_) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation_), glm::vec3(0, 0, 1));
        matrix_.view = glm::inverse(transform);
    }

    [[nodiscard]] const ViewProjection& Matrix() const { return matrix_; }

   private:
    glm::vec3 position_{ 0.0f };
    float rotation_{ 0.0f };
    float aspect_ratio_{ 1.0f };
    float zoom_level_{ 0.0f };
    ViewProjection matrix_{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_H_
