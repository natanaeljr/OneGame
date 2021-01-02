#ifndef FIRSTGAME_RENDER_RENDERER_H_
#define FIRSTGAME_RENDER_RENDERER_H_

#include <entt/entity/fwd.hpp>

namespace firstgame::render {

//! Renderer Interface
class Renderer final {
   public:
    Renderer(int width, int height);
    ~Renderer();

    // Interface
    void Render(const entt::registry& registry);
    void ResizeCanvas(int width, int height);
    void HandleScrollEvent(double yoffset);

    // Copy/Move
    Renderer(Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   private:
    //! Implementation object buffer
    alignas(4) unsigned char impl_[144]{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_RENDERER_H_
