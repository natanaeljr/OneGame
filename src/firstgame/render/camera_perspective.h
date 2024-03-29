////////////////////////////////////////////////////////////////////////////////////////////////////
/// This file defines a Perspective Camera class with an interface that handles input events and
/// generates a resulting ViewProjection matrix for the rendering of a 3D scene.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_
#define FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "firstgame/util/direction.h"
#include "firstgame/util/size.h"
#include "view_projection.h"

namespace firstgame::render {

/// Perspective Camera provides projection of a 3D scene that mimics the way the human eye sees.
/// The interface is designed to respond to input events filtered by a camera system.
/// As result, the View and Projection matrices are generated and can be then retrieved by the
/// Get method for the rendering pass.
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
        forward_ = CalculateForward();
        matrix_.view = CalculateView();
    }

    void Translate(util::MoveDirection direction, float deltatime)
    {
        static constexpr float kSpeed = 15.0f;
        const float velocity = kSpeed * deltatime;
        switch (direction) {
            case util::MoveDirection::Forward:
                position_ += (glm::normalize(glm::vec3(forward_.x, 0.0f, forward_.z)) * velocity);
                break;
            case util::MoveDirection::Backward:
                position_ -= (glm::normalize(glm::vec3(forward_.x, 0.0f, forward_.z)) * velocity);
                break;
            case util::MoveDirection::Left: position_ -= (Right() * velocity); break;
            case util::MoveDirection::Right: position_ += (Right() * velocity); break;
            case util::MoveDirection::Up: position_ += (kWorldUp * velocity); break;
            case util::MoveDirection::Down: position_ -= (kWorldUp * velocity); break;
        }
        matrix_.view = CalculateView();
    }

    [[nodiscard]] const ViewProjection& Matrix() const { return matrix_; }

   private:
    [[nodiscard]] glm::mat4 CalculateView() const
    {
        return glm::lookAt(position_, position_ + forward_, Up());  //
    }
    [[nodiscard]] glm::mat4 CalculateProjection() const
    {
        return glm::perspective(glm::radians(fov_degrees_), aspect_ratio_, 0.1f, 100.0f);
    }
    [[nodiscard]] glm::vec3 CalculateForward() const
    {
        glm::vec3 forward{ 0.0f, 0.0f, -1.0f };
        forward.x = cos(glm::radians(yaw_degrees_)) * cos(glm::radians(pitch_degrees_));
        forward.y = sin(glm::radians(pitch_degrees_));
        forward.z = sin(glm::radians(yaw_degrees_)) * cos(glm::radians(pitch_degrees_));
        return glm::normalize(forward);
    }
    [[nodiscard]] glm::vec3 Right() const { return glm::normalize(glm::cross(forward_, kWorldUp)); }
    [[nodiscard]] glm::vec3 Up() const { return glm::normalize(glm::cross(Right(), forward_)); }

   private:
    static constexpr glm::vec3 kWorldUp{ 0.0f, 1.0f, 0.0f };

   private:
    float aspect_ratio_;
    float fov_degrees_{ 45.0f };
    float yaw_degrees_{ -90.0f };
    float pitch_degrees_{ 0.0f };
    glm::vec3 position_{ 0.0f, 0.0f, 5.0f };
    glm::vec3 forward_{ 0.0f, 0.0f, -1.0f };
    ViewProjection matrix_{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_PERSPECTIVE_H_
