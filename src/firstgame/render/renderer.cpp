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

template<typename Camera>
struct CameraMatrix {
    Camera camera;
    ViewProjection matrix;
};

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
    CameraMatrix<CameraPerspective> camera_;
    util::Scoped<opengl::Shader> shader_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl(int width, int height)
    : camera_([=] {
          CameraPerspective camera = {
              .aspect_ratio = (float) width / (float) height,
              .fovy_degrees = 45.0f,
          };
          ViewProjection matrix = camera.ToViewProjection();
          return CameraMatrix<CameraPerspective>{ .camera = camera, .matrix = matrix };
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

    glUniformMatrix4fv(ShaderUniform::View.location(), 1, GL_FALSE,
                       glm::value_ptr(camera_.matrix.view));
    glUniformMatrix4fv(ShaderUniform::Projection.location(), 1, GL_FALSE,
                       glm::value_ptr(camera_.matrix.projection));

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

void RendererImpl::ResizeCanvas(int width, int height)
{
    glViewport(0, 0, width, height);
    camera_.camera.Resize(width, height);
    camera_.matrix = camera_.camera.ToViewProjection();
}

/**************************************************************************************************/

void RendererImpl::HandleScrollEvent(double yoffset)
{
    camera_.camera.Zoom(static_cast<float>(yoffset));
    camera_.matrix = camera_.camera.ToViewProjection();
    TRACE("Updated fovy_degrees_: {}", camera_.camera.fovy_degrees);
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

}  // namespace firstgame::render
