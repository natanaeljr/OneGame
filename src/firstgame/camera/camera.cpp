#include "camera.h"

#include <entt/entity/registry.hpp>
#include "firstgame/util/size.h"
#include "firstgame/render/transform.h"
#include "firstgame/system/log.h"

namespace firstgame::camera {

using render::Transform;

////////////////////////////////////////////////////////////////////////////////////////////////////

Camera perspective_camera(const Perspective& perspective, const Transform& transform)
{
    return Camera{
        .view =
            [&] {
                glm::mat4 view = glm::mat4_cast(transform.rotation);
                view = glm::translate(view, -transform.position);
                return view;
            }(),
        //                .view = glm::lookAt(transform.position, transform.position +
        //                transform.forward(),
        //                                    transform.up()),
        .projection =
            glm::perspective(glm::radians(perspective.fov_degrees), perspective.aspect_ratio,
                             perspective.z_near, perspective.z_far),
    };
}

void perspective_resize(Perspective& perspective, util::SizeT<float> size)
{
    perspective.aspect_ratio = size.width / size.height;
}

void perspective_zoom(Perspective& perspective, const ZoomVelocity& zoom_velocity, float offset)
{
    perspective.fov_degrees -= offset * zoom_velocity.velocity;
    perspective.fov_degrees = std::max(perspective.fov_degrees, 1.0f);
    perspective.fov_degrees = std::min(perspective.fov_degrees, 45.0f);
}

void perspective_rotation_free_style(Transform& transform, const AngularVelocity& angular,
                                     float xoffset, float yoffset)
{
    glm::vec3 angle = glm::vec3(yoffset, xoffset, 0.0f) * angular.velocity;
    glm::quat pitch_rotation = glm::angleAxis(angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yaw_rotation = glm::angleAxis(angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat roll_rotation = glm::angleAxis(angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
    transform.rotation = yaw_rotation * pitch_rotation * roll_rotation * transform.rotation;
}

void camera_rotation(Transform& transform, const AngularVelocity& angular, float xoffset,
                     float yoffset)
{
    glm::vec3 angle = glm::vec3(yoffset, xoffset, 0.0f) * angular.velocity;
    transform.rotation = glm::rotate(transform.rotation, angle.x, transform.right());
    transform.rotation = glm::rotate(transform.rotation, angle.y, Transform::kWorldUp);
    transform.rotation = glm::rotate(transform.rotation, angle.z, transform.forward());
    transform.rotation = glm::normalize(transform.rotation);

    //    glm::quat right_twist = transform.twist(transform.right());
    float pitch = glm::degrees(transform.pitch());
    float yaw = glm::degrees(transform.yaw());
    float roll = glm::degrees(transform.roll());
    auto right = transform.right();
    TRACE("right: {{x:{}, y:{}, z:{}}}", right.x, right.y, right.z);
    TRACE("pitch: {}, yaw: {}, roll: {}", pitch, yaw, roll);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void updater_perspective_camera(entt::registry& registry)
{
    auto view = registry.view<Camera, const Perspective, const Transform>();
    for (auto [entity, camera, perspective, transform] : view.each()) {
        registry.template replace<Camera>(entity, perspective_camera(perspective, transform));
    }
}

void updater_perspective_zoom(entt::registry& registry)
{
    auto view = registry.view<Camera, const Perspective, const ZoomVelocity>();
    for (auto [entity, camera, perspective, zoom_velocity] : view.each()) {
        registry.patch<Perspective>(entity, [&](auto& perspective) {
            perspective_zoom(perspective, zoom_velocity, 0.0f);  //
        });
    }
}

void event_handler_perspective_zoom(entt::registry& registry, entt::entity entity, float offset)
{
    auto [perspective, zoom_velocity] = registry.get<Perspective, ZoomVelocity>(entity);
    perspective_zoom(perspective, zoom_velocity, offset);
}

void event_handler_camera_rotation(entt::registry& registry, entt::entity entity, float xoffset,
                                   float yoffset)
{
    auto [transform, angular] = registry.get<Transform, AngularVelocity>(entity);
    registry.patch<Transform>(entity, [&](Transform& transform) {
        camera_rotation(transform, angular, xoffset, yoffset);
    });
}

void observer_perspective_updates(entt::registry& registry, entt::entity entity)
{
    auto [camera, perspective] = registry.try_get<Camera, Perspective>(entity);
    if (camera && perspective) {
        registry.patch<Camera>(entity, [&](Camera& camera) {
            camera.projection =
                glm::perspective(glm::radians(perspective->fov_degrees), perspective->aspect_ratio,
                                 perspective->z_near, perspective->z_far);
        });
    }
}

}  // namespace firstgame::camera