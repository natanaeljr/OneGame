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

#include "component.h"
#include "transform.h"

// TODO: temporary
#include "shader_variable.h"
#include "shader.h"

namespace firstgame::renderer {

/**************************************************************************************************/

//! Renderer Implementation
class RendererImpl final {
   public:
    RendererImpl();
    ~RendererImpl();
    void Render(const entt::registry& registry);

   private:
    util::Scoped<opengl::Shader> shader_;
    GLint uniform_modelview_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl()
    : shader_([] {
          using util::filesystem_literals::operator""_path;
          auto& asset_mgr = system::AssetManager::current();
          auto vertex = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
          auto fragment = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
          return opengl::Shader::Build(vertex.data(), fragment.data()).Assert();
      }()),
      uniform_modelview_(glGetUniformLocation(shader_->program, "modelview"))
{
    ASSERT(uniform_modelview_ != -1);
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
        auto modelview = glm::mat4(1.0f);
        modelview = glm::translate(modelview, transform.position);
        modelview = glm::scale(modelview, transform.scale);
        glUniformMatrix4fv(uniform_modelview_, 1, GL_FALSE, glm::value_ptr(modelview));
        glBindVertexArray(render.vao);
        glDrawElements(GL_TRIANGLES, render.elements, GL_UNSIGNED_SHORT, nullptr);
    });
}

/**************************************************************************************************/
/**************************************************************************************************/

Renderer::Renderer()
{
    // guarantee same memory alignment of the interface and implementation
    static_assert(alignof(Renderer) == alignof(RendererImpl));
    // guarantee enough space in the implementation object buffer
    static_assert(sizeof(impl_) == sizeof(RendererImpl));
    // placement new
    new (impl_) RendererImpl();
}

Renderer::~Renderer()
{
    reinterpret_cast<RendererImpl*>(impl_)->~RendererImpl();
}

void Renderer::Render(const entt::registry& registry)
{
    reinterpret_cast<RendererImpl*>(impl_)->Render(registry);
}

}  // namespace firstgame::renderer
