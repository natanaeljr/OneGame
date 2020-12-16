#include "triangle_renderer.h"

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

namespace firstgame::renderer {

/**************************************************************************************************/

TriangleRenderer::TriangleRenderer()
{
    TRACE("Created TriangleRenderer");

    GLuint buffers[2], &vao = buffers[0], &vbo = buffers[1];
    glGenBuffers(1, buffers);
    std::ignore = vao;
    std::ignore = vbo;
}

/**************************************************************************************************/

TriangleRenderer::~TriangleRenderer()
{
    // TODO: Delete VAO, VBO, EBO
    TRACE("Destroyed TriangleRenderer");
}

/**************************************************************************************************/

void TriangleRenderer::Render()
{
    // TODO: Draw Triangle
}

}  // namespace firstgame::renderer
