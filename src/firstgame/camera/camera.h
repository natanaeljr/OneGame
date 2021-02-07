#ifndef FIRSTGAME_CAMERA_CAMERA_H_
#define FIRSTGAME_CAMERA_CAMERA_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <entt/entity/fwd.hpp>

namespace firstgame::camera {

struct Camera final {
    // TODO: mover para ViewProjection
    glm::mat4 view{ 1.0f };
    glm::mat4 projection{ 1.0f };

    // CameraTransform (https://forum.unity.com/threads/solved-how-to-clamp-camera-rotation-on-the-x-axis-fps-controller.526871/)
    // glm::vec3 position{ 0.0f };
    // glm::quat rotation{ glm::vec3(0.0f) };

    // NOTE: we actually have to have 2 transforms and 2 rotations
    // one for the target, computed on each update, and other for the rendering which interpolates
    // Perspective or Orthographic Component
    // Position Component
    // Rotation Component (quaternion) (forward, right, up)
    // Motion Component (for linear interpolation)
    // Control Component (settings)
    // Renderable Component (for rendering a camera model)
    // Rect/Quad/Box/AABB (for checking if the cursor is over the camera viewport)
};

struct Perspective final {
    float aspect_ratio{ 1.0f };
    float fov_degrees{ 45.0f };
    float z_near{ 0.01f };
    float z_far{ 1000.0f };
};

struct Orthographic final {
    float aspect_ratio{ 1.0f };
    float zoom_level{ 0.0f };
    float z_near{ -100.0f };
    float z_far{ +100.0f };
};

struct ZoomVelocity final {
    float velocity;
};

struct LinearVelocity final {
    glm::vec3 velocity{ 1.0f };
};
struct AngularVelocity final {
    glm::vec3 velocity{ 1.0f };
};

void updater_perspective_camera(entt::registry& registry);

void event_handler_camera_rotation(entt::registry& registry, entt::entity entity,
                                        float xoffset, float yoffset);

}  // namespace firstgame::camera

#endif  // FIRSTGAME_CAMERA_CAMERA_H_
