#ifndef FIRSTGAME_RENDER_TRANSFORM_H_
#define FIRSTGAME_RENDER_TRANSFORM_H_

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace firstgame::render {

struct Transform {
    glm::vec3 position{ 0.0f };
    glm::vec3 scale{ 1.0f };
    glm::quat rotation{ glm::vec3(0.0f) };

    // Local Directions
    [[nodiscard]] glm::vec3 forward() const { return glm::conjugate(rotation) * kWorldForward; }
    [[nodiscard]] glm::vec3 right() const
    {
        return glm::normalize(glm::conjugate(rotation) * kWorldRight);
    }
    [[nodiscard]] glm::vec3 up() const { return glm::conjugate(rotation) * kWorldUp; }

    // Local Rotations (in radians)
    [[nodiscard]] float pitch() const { return glm::pitch(pitch_rotation()); }
    [[nodiscard]] float roll() const { return glm::roll(roll_rotation()); }
    [[nodiscard]] float yaw() const { return glm::yaw(yaw_rotation()); }

    [[nodiscard]] glm::quat pitch_rotation() const
    {
        return glm::normalize(glm::quat(rotation.w, glm::vec3(rotation.x, 0.0f, 0.0f)));
    }
    [[nodiscard]] glm::quat roll_rotation() const
    {
        return glm::normalize(glm::quat(rotation.w, glm::vec3(0.0f, 0.0f, rotation.z)));
    }
    [[nodiscard]] glm::quat yaw_rotation() const
    {
        return glm::normalize(glm::quat(rotation.w, glm::vec3(0.0f, rotation.y, 0.0f)));
    }

    /// Use the swing-twist decomposition to get the component of a rotation around the given axis.
    /// https://stackoverflow.com/questions/3684269/component-of-a-quaternion-rotation-around-an-axis
    [[nodiscard]] glm::quat twist(const glm::vec3& direction) const
    {
        glm::vec3 rotation_axis = glm::vec3(rotation.x, rotation.y, rotation.z);
        float dot = glm::dot(direction, rotation_axis);
        glm::vec3 projection = direction * dot;
        glm::quat twist = glm::normalize(glm::quat(rotation.w, projection));
        if (dot < 0.0) {
            twist = -twist;
        }
        return twist;
    }

    // World Directions
    static constexpr glm::vec3 kWorldForward = { 0.0f, 0.0f, -1.0f };
    static constexpr glm::vec3 kWorldRight = { 1.0f, 0.0f, 0.0f };
    static constexpr glm::vec3 kWorldUp = { 0.0f, 1.0f, 0.0f };
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_TRANSFORM_H_
