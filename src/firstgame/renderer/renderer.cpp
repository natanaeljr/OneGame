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
    void HandleScrollEvent(double yoffset);

   private:
    int width_, height_;
    float fovy_degrees_;
    glm::mat4 projection_;
    util::Scoped<opengl::Shader> shader_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl(int width, int height)
    : width_(width),
      height_(height),
      fovy_degrees_(45.0f),
      projection_([=] {
          float aspect_ratio = (float) width_ / (float) height_;
          return glm::perspective(glm::radians(fovy_degrees_), aspect_ratio, +1.0f, -1.0f);
      }()),
      shader_([] {
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

    {
        auto view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glUniformMatrix4fv(ShaderUniform::View.location(), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(ShaderUniform::Projection.location(), 1, GL_FALSE,
                           glm::value_ptr(projection_));
    }

    auto view = registry.view<const TransformComponent, const RenderComponent>();
    view.each([this](const auto& transform, const auto& render) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, transform.scale);
        glUniformMatrix4fv(ShaderUniform::Model.location(), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(render.vao);
        glDrawElements(GL_TRIANGLES, render.elements, GL_UNSIGNED_SHORT, nullptr);
    });
}

/**************************************************************************************************/

void RendererImpl::ResizeCanvas(int width, int height)
{
    this->width_ = width;
    this->height_ = height;
    float aspect_ratio = (float) width_ / (float) height_;
    projection_ = glm::perspective(glm::radians(45.0f), aspect_ratio, +1.0f, -1.0f);
    glViewport(0, 0, width, height);
}

/**************************************************************************************************/

void RendererImpl::HandleScrollEvent(double yoffset)
{
    fovy_degrees_ -= (float) yoffset * 2.0f;
    if (fovy_degrees_ < 1.0f)
        fovy_degrees_ = 1.0f;
    if (fovy_degrees_ > 45.0f)
        fovy_degrees_ = 45.0f;

    float aspect_ratio = (float) width_ / (float) height_;
    projection_ = glm::perspective(glm::radians(fovy_degrees_), aspect_ratio, +1.0f, -1.0f);

    TRACE("Updated fovy_degrees_: {}", fovy_degrees_);
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

void Renderer::HandleScrollEvent(double yoffset)
{
    reinterpret_cast<RendererImpl*>(impl_)->HandleScrollEvent(yoffset);
}

}  // namespace firstgame::renderer
