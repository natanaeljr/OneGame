#ifndef FIRSTGAME_RENDER_RENDERER_H_
#define FIRSTGAME_RENDER_RENDERER_H_

#include <entt/entity/fwd.hpp>
#include "firstgame/util/size.h"
#include "firstgame/event/key.h"

namespace firstgame::render {

//! Renderer Interface
class Renderer final {
   public:
    Renderer(entt::registry &registry, util::Size window_size);
    ~Renderer();

    // Interface
    void Render(entt::registry& registry);
    void OnResize(util::Size size);
    void OnScroll(float offset);
    void OnCursorMove(float xpos, float ypos);
    void OnKeystroke(event::KeyEvent key_event, float deltatime);

    // Copy/Move
    Renderer(Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   private:
    //! Implementation object buffer
    alignas(8) unsigned char impl_[1024]{};
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_RENDERER_H_
