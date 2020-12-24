#include "renderer.h"

#include <new>
#include <string_view>
#include "triangle_renderer.h"
#include "firstgame/opengl/gl.h"
#include "firstgame/opengl/shader.h"
#include "firstgame/system/log.h"
#include "firstgame/system/asset_mgr.h"
#include "firstgame/util/scoped.h"
#include "firstgame/util/filesystem_literals.h"

namespace firstgame::renderer {

/**************************************************************************************************/

//! Renderer Implementation
class RendererImpl final {
   public:
    RendererImpl();
    ~RendererImpl();
    void Render();

   private:
    util::Scoped<opengl::Shader> shader_;
    TriangleRenderer triangle_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl()
    : shader_([] {
          using util::filesystem_literals::operator""_path;
          auto& asset_mgr = system::AssetManager::current();
          auto vertex = asset_mgr.Open("shaders"_path / "main.vert"_path).Assert()->ReadToString();
          auto fragment = asset_mgr.Open("shaders"_path / "main.frag"_path).Assert()->ReadToString();
          return opengl::Shader::Build(vertex.data(), fragment.data()).Assert();
      }())
{
    TRACE("Created RendererImpl");
}

/**************************************************************************************************/

RendererImpl::~RendererImpl()
{
    TRACE("Destroyed RendererImpl");
}

/**************************************************************************************************/

void RendererImpl::Render()
{
    glUseProgram(shader_->program);

    triangle_.Render();

    glUseProgram(0);
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

void Renderer::Render()
{
    reinterpret_cast<RendererImpl*>(impl_)->Render();
}

}  // namespace firstgame::renderer
