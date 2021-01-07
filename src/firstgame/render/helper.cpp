#include "helper.h"

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

// TODO: temporary
#include "shader_variables.h"

namespace firstgame::render {

/**************************************************************************************************/

// Move this to a Painter/Designer of common polygons
// The painter should be by Draw mode (triangles, triangles strip, etc) because of the indices.

Renderable GenerateQuad()
{
    static constexpr GLfloat vertices[] = {
        // X      Y      Z
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

Renderable GenerateCube()
{
    // (-1,+1)       (+1,+1)
    //  Y ^ - - - - - - o
    //    |  B       D  |
    //    |   +-----+   |
    //    |   | \   |   |
    //    |   |  0  |   |
    //    |   |   \ |   |
    //    |   +-----+   |
    //    |  A       C  |
    //    o - - - - - - > X
    // (-1,-1)       (+1,-1)
    // positive Z goes through screen towards you
    static constexpr GLfloat vertices[] = {
        // X      Y      Z
        -1.0f, -1.0f, +1.0f,  // [0] A front
        -1.0f, +1.0f, +1.0f,  // [1] B front
        +1.0f, -1.0f, +1.0f,  // [2] C front
        +1.0f, +1.0f, +1.0f,  // [3] D front
        -1.0f, -1.0f, -1.0f,  // [4] A back
        -1.0f, +1.0f, -1.0f,  // [5] B back
        +1.0f, -1.0f, -1.0f,  // [6] C back
        +1.0f, +1.0f, -1.0f,  // [7] D back
    };
    static constexpr GLushort indices[] = {
        0, 1, 2, 2, 1, 3,  // Front
        4, 5, 6, 6, 5, 7,  // Back
        1, 5, 3, 3, 5, 7,  // Top
        0, 4, 2, 2, 4, 6,  // Bottom
        4, 5, 0, 0, 5, 1,  // Left
        6, 7, 2, 2, 7, 3,  // Right
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