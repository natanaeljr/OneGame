#include "renderable.h"

#include "firstgame/opengl/gl/functions.h"

namespace firstgame::render {

Renderable::Renderable(unsigned short elements) : num_vertices(elements)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Renderable::~Renderable()
{
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (ebo)
        glDeleteBuffers(1, &ebo);
}

}  // namespace firstgame::render
