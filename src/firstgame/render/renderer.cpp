#include "renderer.h"

#include <new>
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
#include "transform.h"
#include "shader_variables.h"
#include "camera_system.h"

namespace firstgame::render {

/**************************************************************************************************/

//! Renderer Implementation
class RendererImpl final {
   public:
    explicit RendererImpl(util::Size size);
    ~RendererImpl();
    void Render(const entt::registry& registry);
    void OnResize(util::Size size);
    void OnZoom(float offset);
    void OnCursorMove(float xpos, float ypos);
    void OnKeystroke(event::KeyEvent key_event, float deltatime);

   private:
    CameraSystem camera_;
    util::Scoped<opengl::Shader> shader_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl(util::Size size)
    : camera_(size), shader_([] {
          using util::filesystem_literals::operator""_path;
          auto& asset_mgr = system::AssetManager::current();
          auto vertex = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
          auto fragment = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
          return opengl::Shader::Build(vertex.data(), fragment.data()).Assert();
      }())
{
    OnResize(size);
    TRACE("Created RendererImpl");
}

/**************************************************************************************************/

RendererImpl::~RendererImpl()
{
    TRACE("Destroyed RendererImpl");
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
    // program
    glUseProgram(shader_->program);
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
        glDrawElements(GL_TRIANGLES, renderable.num_vertices, GL_UNSIGNED_SHORT, nullptr);
    });
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

Renderer::Renderer(util::Size size)
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
