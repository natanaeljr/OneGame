#ifndef FIRSTGAME_RENDER_RENDERER_H_
#define FIRSTGAME_RENDER_RENDERER_H_

#include <entt/entity/fwd.hpp>
#include "../util/size.h"

namespace firstgame::render {

//! Renderer Interface
class Renderer final {
   public:
    Renderer(util::Size size);
    ~Renderer();

    // Interface
    void Render(const entt::registry& registry);
    void OnResize(util::Size size);
    void OnZoom(float offset);

    // Copy/Move
    Renderer(Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   private:
    //! Implementation object buffer
    alignas(4) unsigned char impl_[1024]{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_RENDERER_H_
