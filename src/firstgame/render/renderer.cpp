#include "renderer.h"

#include <new>
#include <optional>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entt/entity/registry.hpp>

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

namespace firstgame::render {

using opengl::Shader;
using util::Scoped;
using util::Size;

/**************************************************************************************************/

//! Renderer Implementation
class RendererImpl final {
   public:
    explicit RendererImpl(Size size);
    ~RendererImpl();
    void Render(const entt::registry& registry);
    void OnResize(Size size);
    void OnZoom(float offset);
    void OnCursorMove(float xpos, float ypos);
    void OnKeystroke(event::KeyEvent key_event, float deltatime);

   private:
    CameraSystem camera_;
    Scoped<Shader> shader_main_{};
    Scoped<Shader> shader_instance_{};
};

/**************************************************************************************************/

RendererImpl::RendererImpl(Size size) : camera_(size)
{
    using util::filesystem_literals::operator""_path;
    auto& asset_mgr = system::AssetManager::current();

    auto main_vert = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
    auto main_frag = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
    auto instance_vert = asset_mgr.Open("shaders"_path / "instance.vert").Assert()->ReadToString();

    shader_main_ = Shader::Build("main", { main_vert, main_frag }).Assert();
    shader_instance_ = Shader::Build("instance", { instance_vert, main_frag }).Assert();

    OnResize(size);

    TRACE("Created RendererImpl");
}

/**************************************************************************************************/

RendererImpl::~RendererImpl()
{
    TRACE("Destroying RendererImpl");
}

/**************************************************************************************************/

void RendererImpl::Render(const entt::registry& registry)
{
    // settings
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // clear buffers
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
        // id
        shader_main_->Bind();
        // uniforms
        camera_.Render(RenderPass::_3D);
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
        // id
        shader_instance_->Bind();
        // uniforms
        camera_.Render(RenderPass::_3D);
        // objects
        auto view = registry.view<const RenderableInstanced>();
        view.each([](const RenderableInstanced& renderable) {
            glBindVertexArray(renderable.vao);
            glDrawElementsInstanced(GL_TRIANGLES, renderable.num_indices, GL_UNSIGNED_SHORT, nullptr,
                                    renderable.num_instances);
        });
    }
    // undo
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**************************************************************************************************/

void RendererImpl::OnResize(Size size)

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
}

/**************************************************************************************************/

void RendererImpl::OnKeystroke(event::KeyEvent key_event, float deltatime)
{
    using input::Key;
    using input::KeyAction;
    using util::MoveDirection;

    const auto move_direction = [&]() -> std::optional<MoveDirection> {
        if (key_event.action == KeyAction::Release) {
            return std::nullopt;
        }
        switch (key_event.key) {
            case Key::W: return MoveDirection::Forward;
            case Key::S: return MoveDirection::Backward;
            case Key::A: return MoveDirection::Left;
            case Key::D: return MoveDirection::Right;
            case Key::SPACE: return MoveDirection::Up;
            case Key::TAB: return MoveDirection::Down;
            default: return std::nullopt;
        }
    }();

    if (move_direction) {
        camera_.OnMove(*move_direction, deltatime);
    }
}

/**************************************************************************************************/
/**************************************************************************************************/

Renderer::Renderer(Size size)
{
    // guarantee same memory alignment of the interface and implementation
    static_assert(alignof(Renderer) == alignof(RendererImpl));
    // guarantee enough space in the implementation object buffer
    static_assert(sizeof(impl_) >= sizeof(RendererImpl));
    // placement new
    new (impl_) RendererImpl(size);
}

Renderer::~Renderer()
{
    reinterpret_cast<RendererImpl*>(impl_)->~RendererImpl();
}

void Renderer::Render(const entt::registry& registry)
{
    reinterpret_cast<RendererImpl*>(impl_)->Render(registry);
}

void Renderer::OnResize(Size size)
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
