#ifndef FIRSTGAMELINUX_TRIANGLE_RENDERER_H_
#define FIRSTGAMELINUX_TRIANGLE_RENDERER_H_

#include "firstgame/opengl/gl.h"

namespace firstgame::renderer {

class TriangleRenderer {
   public:
    TriangleRenderer();
    ~TriangleRenderer();

    void Render();

   private:
    GLuint vao_, vbo_;
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAMELINUX_TRIANGLE_RENDERER_H_
