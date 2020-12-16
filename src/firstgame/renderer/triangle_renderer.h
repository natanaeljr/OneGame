#ifndef FIRSTGAMELINUX_TRIANGLE_RENDERER_H_
#define FIRSTGAMELINUX_TRIANGLE_RENDERER_H_

#include <glad/glad.h>

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
