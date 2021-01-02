#include "renderer.h"

#include <new>
#include <string_view>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entt/entity/registry.hpp>

#include "firstgame/opengl/gl.h"
#include "firstgame/opengl/shader.h"
#include "firstgame/system/log.h"
#include "firstgame/system/asset_mgr.h"
#include "firstgame/util/scoped.h"
#include "firstgame/util/filesystem_literals.h"

#include "camera_orthographic.h"
#include "camera_perspective.h"
#include "view_projection.h"
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

   private:
    util::Scoped<opengl::Shader> shader_;
    CameraSystem camera_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl(util::Size size)
    : shader_([] {
          using util::filesystem_literals::operator""_path;
          auto& asset_mgr = system::AssetManager::current();
          auto vertex = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
          auto fragment = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
          return opengl::Shader::Build(vertex.data(), fragment.data()).Assert();
      }()),
      camera_(size)
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
    glUseProgram(shader_->program);

    camera_.Render(RenderPass::_3D);

    auto view = registry.view<const Transform, const Renderable>();
    view.each([this](const auto& transform, const auto& renderable) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, transform.scale);
        glUniformMatrix4fv(ShaderUniform::Model.location(), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(renderable.vao);
        glDrawElements(GL_TRIANGLES, renderable.num_vertices, GL_UNSIGNED_SHORT, nullptr);
    });
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

void Renderer::OnZoom(float offset)
{
    reinterpret_cast<RendererImpl*>(impl_)->OnZoom(offset);
}

}  // namespace firstgame::render
