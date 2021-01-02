#include "helper.h"

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

// TODO: temporary
#include "shader_variables.h"

namespace firstgame::render {

/**************************************************************************************************/

// Move this to a Painter/Designer of common polygons
Renderable GenerateQuad()
{
    static constexpr GLfloat vertices[] = {
        -1.0f, -1.0f, +0.0f,  //
        -1.0f, +1.0f, +0.0f,  //
        +1.0f, -1.0f, +0.0f,  //
        +1.0f, +1.0f, +0.0f,  //
    };
    static constexpr GLushort indices[] = {
        0, 1, 2,  //
        2, 1, 3,  //
    };
    return GenerateRenderable(vertices, indices);
}

/**************************************************************************************************/

Renderable GenerateRenderable(gsl::span<const float> vertices,
                              gsl::span<const unsigned short> indices)
{
    ASSERT(indices.size() <= std::numeric_limits<unsigned short>::max());

    Renderable renderable{ static_cast<unsigned short>(indices.size()) };
    glBindVertexArray(renderable.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(ShaderVertexAttrib::Position.location(), 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(ShaderVertexAttrib::Position.location());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
    return renderable;
}

}  // namespace firstgame::render