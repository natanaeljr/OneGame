#include "renderer.h"

#include <new>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/observer.hpp>

#include "firstgame/opengl/gl.h"
#include "firstgame/opengl/shader.h"
#include "firstgame/system/log.h"
#include "firstgame/system/asset_mgr.h"
#include "firstgame/util/scoped.h"
#include "firstgame/util/filesystem_literals.h"

#include "camera_perspective.h"
#include "renderable.h"
#include "renderable_instanced.h"
#include "transform.h"
#include "shader_variables.h"
#include "camera_system.h"

#include "firstgame/camera/camera.h"

namespace firstgame::render {

/**************************************************************************************************/

//! Renderer Implementation
class RendererImpl final {
   public:
    explicit RendererImpl(entt::registry& registry, util::Size window_size);
    ~RendererImpl();
    void Render(entt::registry& registry);
    void OnResize(util::Size size);
    void OnZoom(float offset);
    void OnCursorMove(float xpos, float ypos);
    void OnKeystroke(event::KeyEvent key_event, float deltatime);

   private:
    entt::registry& registry_;
    CameraSystem camera_;
    entt::entity camera_entity_;
    util::Scoped<opengl::Shader> shader_main_{};
    util::Scoped<opengl::Shader> shader_instance_{};
};

/**************************************************************************************************/

RendererImpl::RendererImpl(entt::registry& registry, util::Size window_size)
    : registry_(registry), camera_(window_size)
{
    // shaders
    using util::filesystem_literals::operator""_path;
    auto& asset_mgr = system::AssetManager::current();
    auto main_vert = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
    auto main_frag = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
    auto instance_vert = asset_mgr.Open("shaders"_path / "instance.vert").Assert()->ReadToString();
    shader_main_ = opengl::Shader::Build(main_vert.data(), main_frag.data()).Assert();
    shader_instance_ = opengl::Shader::Build(instance_vert.data(), main_frag.data()).Assert();

    // setup
    OnResize(window_size);

    // camera
    entt::handle camera{ registry, registry.create() };
    camera.emplace<camera::Camera>();
    camera.emplace<camera::Perspective>(camera::Perspective{
        .aspect_ratio = util::SizeT<float>(window_size).aspect_ratio(),
        .fov_degrees = 45.0f,
        .z_near = 0.01f,
        .z_far = 1000.0f,
    });
    camera.emplace<render::Transform>(render::Transform{
        .position = { 0.0f, 0.0f, 30.0f },
        .scale = glm::vec3(1.0f),
        .rotation{ glm::vec3(0.0f) },
    });
    camera.emplace<camera::AngularVelocity>(camera::AngularVelocity{
        .velocity = glm::vec3(1.0f),
    });

    camera_entity_ = camera.entity();

    // entt::observer observer{ registry, entt::basic_collector<>::update<camera::Camera>() };

    // camera::event_handler_camera_rotation(registry, camera_entity_, 0.0f, 0.5f);
    camera::updater_perspective_camera(registry);

    //    observer.each(
    //        [](auto entity) { TRACE("Observed camera::Camera update on entity {}", entity); });

    TRACE("Created RendererImpl");
}

/**************************************************************************************************/

RendererImpl::~RendererImpl()
{
    TRACE("Destroyed RendererImpl");
}

/**************************************************************************************************/

void RendererImpl::Render(entt::registry& registry)
{
    auto RenderCamera = [&] {
        auto camera = registry.get<camera::Camera>(camera_entity_);
        glUniformMatrix4fv(ShaderUniform::View.location(), 1, GL_FALSE,
                           glm::value_ptr(camera.view));
        glUniformMatrix4fv(ShaderUniform::Projection.location(), 1, GL_FALSE,
                           glm::value_ptr(camera.projection));
    };

    // settings
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // clear buffers
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
        // program
        glUseProgram(shader_main_->program);
        // uniforms
        // camera_.Render(RenderPass::_3D);
        RenderCamera();
        // objects
        auto view = registry.view<const Transform, const Renderable>();
        view.each([](const Transform& transform, const Renderable& renderable) {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.position);
            glm::mat4 rotation = glm::toMat4(transform.rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);
            glm::mat4 model = translation * rotation * scale;
            glUniformMatrix4fv(ShaderUniform::Model.location(), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, renderable.num_indices, GL_UNSIGNED_SHORT, nullptr);
        });
    }
    {
        // program
        glUseProgram(shader_instance_->program);
        // uniforms
        // camera_.Render(RenderPass::_3D);
        RenderCamera();
        // objects
        auto view = registry.view<const RenderableInstanced>();
        view.each([](const RenderableInstanced& renderable) {
            glBindVertexArray(renderable.vao);
            glDrawElementsInstanced(GL_TRIANGLES, renderable.num_indices, GL_UNSIGNED_SHORT,
                                    nullptr, renderable.num_instances);
        });
    }
    // undo
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**************************************************************************************************/

void RendererImpl::OnResize(util::Size size)

{
    glViewport(0, 0, size.width, size.height);
    camera_.OnResize(size);
}

/**************************************************************************************************/

void RendererImpl::OnZoom(float offset)
{
    camera_.OnZoom(offset);
}

/**************************************************************************************************/

void RendererImpl::OnCursorMove(float xpos, float ypos)
{
    camera_.OnPoint(xpos, ypos);

    static float last_xpos = xpos;
    static float last_ypos = ypos;

    float xoffset = xpos - last_xpos;
    float yoffset = ypos - last_ypos;

    camera::event_handler_camera_rotation(registry_, camera_entity_, glm::radians(xoffset * 0.1f),
                                          glm::radians(yoffset * 0.1f));
    camera::updater_perspective_camera(registry_);

    last_xpos = xpos;
    last_ypos = ypos;
}

/**************************************************************************************************/

void RendererImpl::OnKeystroke(event::KeyEvent key_event, float deltatime)
{
    const auto move_direction = [&]() -> std::optional<util::MoveDirection> {
        if (key_event.action == input::KeyAction::Release) {
            return std::nullopt;
        }
        switch (key_event.key) {
            case input::Key::W: return util::MoveDirection::Forward;
            case input::Key::S: return util::MoveDirection::Backward;
            case input::Key::A: return util::MoveDirection::Left;
            case input::Key::D: return util::MoveDirection::Right;
            case input::Key::SPACE: return util::MoveDirection::Up;
            case input::Key::TAB: return util::MoveDirection::Down;
            default: return std::nullopt;
        }
    }();
    if (move_direction) {
        camera_.OnMove(*move_direction, deltatime);
    }
}

/**************************************************************************************************/
/**************************************************************************************************/

Renderer::Renderer(entt::registry& registry, util::Size window_size)
{
    // guarantee same memory alignment of the interface and implementation
    static_assert(alignof(Renderer) == alignof(RendererImpl));
    // guarantee enough space in the implementation object buffer
    static_assert(sizeof(impl_) >= sizeof(RendererImpl));
    // placement new
    new (impl_) RendererImpl(registry, window_size);
}

Renderer::~Renderer()
{
    reinterpret_cast<RendererImpl*>(impl_)->~RendererImpl();
}

void Renderer::Render(entt::registry& registry)
{
    reinterpret_cast<RendererImpl*>(impl_)->Render(registry);
}

void Renderer::OnResize(util::Size size)
{
    reinterpret_cast<RendererImpl*>(impl_)->OnResize(size);
}

void Renderer::OnScroll(float offset)
{
    reinterpret_cast<RendererImpl*>(impl_)->OnZoom(offset);
}

void Renderer::OnCursorMove(float xpos, float ypos)
{
    reinterpret_cast<RendererImpl*>(impl_)->OnCursorMove(xpos, ypos);
}

void Renderer::OnKeystroke(event::KeyEvent key_event, float deltatime)
{
    reinterpret_cast<RendererImpl*>(impl_)->OnKeystroke(key_event, deltatime);
}

}  // namespace firstgame::render
