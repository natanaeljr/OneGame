#include "renderer.h"

#include <new>
#include <string_view>
#include "triangle_renderer.h"
#include "firstgame/opengl/gl.h"
#include "firstgame/opengl/shader.h"
#include "firstgame/system/log.h"
#include "firstgame/util/scoped.h"

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
          static constexpr std::string_view vextex_src = R"(#version 330 core
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position, 1.0);
}
)";
          static constexpr std::string_view fragment_src = R"(#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";
          auto shader = opengl::Shader::Build(vextex_src.data(), fragment_src.data());
          ASSERT(shader);
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
