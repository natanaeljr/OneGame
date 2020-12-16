#include "renderer.h"

#include <new>
#include "triangle_renderer.h"
#include "firstgame/system/log.h"

namespace firstgame::renderer {

/**************************************************************************************************/

class RendererImpl final {
   public:
    RendererImpl();
    ~RendererImpl();
    void Render();

   private:
    TriangleRenderer triangle_;
};

/**************************************************************************************************/

RendererImpl::RendererImpl()
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
    triangle_.Render();
}

/**************************************************************************************************/
/**************************************************************************************************/

Renderer::Renderer()
{
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
