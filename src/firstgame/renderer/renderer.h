/**
 * Render Interface of FirstGame.
 */

#ifndef FIRSTGAME_RENDERER_RENDERER_H_
#define FIRSTGAME_RENDERER_RENDERER_H_

namespace firstgame::renderer {

//! Renderer Interface
class Renderer final {
   public:
    Renderer();
    ~Renderer();
    void Render();

   private:
    //! Implementation object buffer
    alignas(4) unsigned char impl_[16]{};
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_RENDERER_H_
