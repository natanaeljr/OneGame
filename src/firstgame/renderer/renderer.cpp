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

#include "camera.h"
#include "component.h"
#include "transform.h"
#include "shader_variables.h"

namespace firstgame::renderer {

/**************************************************************************************************/

//! Renderer Implementation
class RendererImpl final {
   public:
    RendererImpl(int width, int height);
    ~RendererImpl();
    void Render(const entt::registry& registry);
    void ResizeCanvas(int width, int height);

   private:
    util::Scoped<opengl::Shader> shader_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl(int width, int height)
    : shader_([] {
          using util::filesystem_literals::operator""_path;
          auto& asset_mgr = system::AssetManager::current();
          auto vertex = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
          auto fragment = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
          return opengl::Shader::Build(vertex.data(), fragment.data()).Assert();
      }())
{
    ResizeCanvas(width, height);
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

    auto view = registry.view<const TransformComponent, const RenderComponent>();
    view.each([this](const auto& transform, const auto& render) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, transform.scale);
        auto view = glm::mat4(1.0f);
        glUniformMatrix4fv(ShaderUniform::Model.location(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(ShaderUniform::View.location(), 1, GL_FALSE, glm::value_ptr(view));
        glBindVertexArray(render.vao);
        glDrawElements(GL_TRIANGLES, render.elements, GL_UNSIGNED_SHORT, nullptr);
    });
}

/**************************************************************************************************/

void RendererImpl::ResizeCanvas(int width, int height)
{
    glViewport(0, 0, width, height);
}

/**************************************************************************************************/
/**************************************************************************************************/

Renderer::Renderer(int width, int height)
{
    // guarantee same memory alignment of the interface and implementation
    static_assert(alignof(Renderer) == alignof(RendererImpl));
    // guarantee enough space in the implementation object buffer
    static_assert(sizeof(impl_) == sizeof(RendererImpl));
    // placement new
    new (impl_) RendererImpl(width, height);
}

Renderer::~Renderer()
{
    reinterpret_cast<RendererImpl*>(impl_)->~RendererImpl();
}

void Renderer::Render(const entt::registry& registry)
{
    reinterpret_cast<RendererImpl*>(impl_)->Render(registry);
}

void Renderer::ResizeCanvas(int width, int height)
{
    reinterpret_cast<RendererImpl*>(impl_)->ResizeCanvas(width, height);
}

}  // namespace firstgame::renderer
