#include "helper.h"

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

namespace firstgame::renderer {

/**************************************************************************************************/

RenderComponent GenerateQuad()
{
    static constexpr GLfloat vertices[] = {
        -0.5f, -0.5f, +0.0f,  //
        -0.5f, +0.5f, +0.0f,  //
        +0.5f, -0.5f, +0.0f,  //
        +0.5f, +0.5f, +0.0f,  //
    };
    static constexpr GLushort indices[] = {
        0, 1, 2,  //
        2, 1, 3,  //
    };
    return GenerateRenderComponent(vertices, indices);
}

/**************************************************************************************************/

RenderComponent GenerateRenderComponent(gsl::span<const GLfloat> vertices,
                                        gsl::span<const GLushort> indices)
{
    ASSERT(indices.size() <= std::numeric_limits<unsigned short>::max());
    RenderComponent render{ static_cast<unsigned short>(indices.size()) };
    glBindVertexArray(render.vao);
    glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
    return render;
}

}  // namespace firstgame::renderer