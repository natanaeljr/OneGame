#include "triangle_renderer.h"

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

namespace firstgame::renderer {

/**************************************************************************************************/

TriangleRenderer::TriangleRenderer()
{
    TRACE("Created TriangleRenderer");

    static constexpr GLfloat vertices[] = {
        -0.5f, -0.5f, +0.0f,  //
        +0.0f, +0.5f, +0.0f,  //
        +0.5f, -0.5f, +0.0f,  //
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

/**************************************************************************************************/

TriangleRenderer::~TriangleRenderer()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);

    TRACE("Destroyed TriangleRenderer");
}

/**************************************************************************************************/

void TriangleRenderer::Render()
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}  // namespace firstgame::renderer
