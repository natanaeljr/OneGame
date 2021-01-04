#ifndef FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_
#define FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "firstgame/util/direction.h"
#include "firstgame/util/size.h"
#include "view_projection.h"

namespace firstgame::render {

class CameraPerspective final {
   public:
    explicit CameraPerspective(util::SizeT<float> size)
        : aspect_ratio_(size.width / size.height),
          matrix_{ .view = CalculateView(), .projection = CalculateProjection() }
    {
    }

    void Resize(util::SizeT<float> size)
    {
        aspect_ratio_ = size.width / size.height;
        matrix_.projection = CalculateProjection();
    }

    void Zoom(float offset)
    {
        static constexpr float kZoomSpeed = 2.0f;
        fov_degrees_ -= offset * kZoomSpeed;
        fov_degrees_ = std::min(fov_degrees_, 45.0f);
        fov_degrees_ = std::max(fov_degrees_, 1.0f);
        matrix_.projection = CalculateProjection();
    }

    void Rotate(float xoffset, float yoffset)
    {
        static constexpr float kSensitivity = 0.1f;
        yaw_degrees_ += xoffset * kSensitivity;
        pitch_degrees_ += yoffset * kSensitivity;
        // constrain pitch, so the screen doesn't get flipped
        pitch_degrees_ = std::min(pitch_degrees_, +89.0f);
        pitch_degrees_ = std::max(pitch_degrees_, -89.0f);
        // consolidate directional vectors
        static constexpr glm::vec3 kWorldUp{ 0.0f, 1.0f, 0.0f };
        front_.x = cos(glm::radians(yaw_degrees_)) * cos(glm::radians(pitch_degrees_));
        front_.y = sin(glm::radians(pitch_degrees_));
        front_.z = sin(glm::radians(yaw_degrees_)) * cos(glm::radians(pitch_degrees_));
        front_ = glm::normalize(front_);
        right_ = glm::normalize(glm::cross(front_, kWorldUp));
        up_ = glm::normalize(glm::cross(right_, front_));
        matrix_.view = CalculateView();
    }

    void Translate(util::MoveDirection direction, float deltatime)
    {
        static constexpr float kSpeed = 8.0f;
        const float velocity = kSpeed * deltatime;
        switch (direction) {
            case util::MoveDirection::Forward: position_ += (front_ * velocity); break;
            case util::MoveDirection::Backward: position_ -= (front_ * velocity); break;
            case util::MoveDirection::Left: position_ -= (right_ * velocity); break;
            case util::MoveDirection::Right: position_ += (right_ * velocity); break;
            case util::MoveDirection::Up: position_ += (up_ * velocity); break;
            case util::MoveDirection::Down: position_ -= (up_ * velocity); break;
        }
        matrix_.view = CalculateView();
    }

    [[nodiscard]] const ViewProjection& Matrix() const { return matrix_; }

   private:
    glm::mat4 CalculateView() const
    {
        return glm::lookAt(position_, position_ + front_, up_);  //
    }
    glm::mat4 CalculateProjection() const
    {
        return glm::perspective(glm::radians(fov_degrees_), aspect_ratio_, +1.0f, -1.0f);
    }

   private:
    float aspect_ratio_;
    float fov_degrees_{ 45.0f };
    float yaw_degrees_{ -90.0f };
    float pitch_degrees_{ 0.0f };
    glm::vec3 position_{ 0.0f, 0.0f, 5.0f };
    glm::vec3 front_{ 0.0f, 0.0f, -1.0f };
    glm::vec3 right_{ 0.0f, 0.0f, 0.0f };
    glm::vec3 up_{ 0.0f, 1.0f, 0.0f };
    ViewProjection matrix_{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_
