#include "renderer.h"

#include <new>
#include <string_view>
#include "triangle_renderer.h"
#include "firstgame/opengl/gl/gl.h"
#include "firstgame/opengl/shader.h"
#include "firstgame/system/log.h"
#include "firstgame/system/asset.h"
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
          auto vertex_asset = asset_mgr.Open("shaders"_path / "main.vert"_path);
          auto fragment_asset = asset_mgr.Open("shaders"_path / "main.frag"_path);
          ASSERT(vertex_asset);
          ASSERT(fragment_asset);
          std::string vertex_src = vertex_asset->ReadToString();
          std::string fragment_src = fragment_asset->ReadToString();
          auto shader = opengl::Shader::Build(vertex_src.data(), fragment_src.data());
          ASSERT(shader.exists());
          return shader;
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
