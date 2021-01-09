#include "helper.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

#include "shader_variables.h"

namespace firstgame::render {

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

Renderable GenerateRenderable(gsl::span<const Vertex> vertices,
                              gsl::span<const unsigned short> indices);

/**************************************************************************************************/

// Move this to a Painter/Designer of common polygons
// The painter should be by Draw mode (triangles, triangles strip, etc) because of the indices.

Renderable GenerateQuad()
{
    static constexpr Vertex vertices[] = {
        // clang-format off
        { .position = { -1.0f, -1.0f, +0.0f }, .color = { 0.0f, 0.5f, 0.5f, 1.0f } },
        { .position = { -1.0f, +1.0f, +0.0f }, .color = { 0.5f, 0.5f, 0.0f, 1.0f } },
        { .position = { +1.0f, -1.0f, +0.0f }, .color = { 0.0f, 1.0f, 1.0f, 1.0f } },
        { .position = { +1.0f, +1.0f, +0.0f }, .color = { 1.0f, 1.5f, 0.0f, 1.0f } },
        // clang-format on
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
    static constexpr Vertex vertices[] = {
        // clang-format off
        { .position = { -1.0f, -1.0f, +1.0f }, .color = { 0.0f, 0.5f, 0.5f, 1.0f } },  // [0] A front
        { .position = { -1.0f, +1.0f, +1.0f }, .color = { 0.5f, 0.5f, 0.0f, 1.0f } },  // [1] B front
        { .position = { +1.0f, -1.0f, +1.0f }, .color = { 0.0f, 1.0f, 0.0f, 1.0f } },  // [2] C front
        { .position = { +1.0f, +1.0f, +1.0f }, .color = { 0.5f, 0.0f, 0.5f, 1.0f } },  // [3] D front
        { .position = { -1.0f, -1.0f, -1.0f }, .color = { 0.0f, 0.0f, 1.0f, 1.0f } },  // [4] A back
        { .position = { -1.0f, +1.0f, -1.0f }, .color = { 1.0f, 0.0f, 0.0f, 1.0f } },  // [5] B back
        { .position = { +1.0f, -1.0f, -1.0f }, .color = { 1.0f, 0.0f, 1.0f, 1.0f } },  // [6] C back
        { .position = { +1.0f, +1.0f, -1.0f }, .color = { 1.0f, 1.0f, 0.0f, 1.0f } },  // [7] D back
        // clang-format on
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

Renderable GenerateRenderable(gsl::span<const Vertex> vertices,
                              gsl::span<const unsigned short> indices)
{
    ASSERT(indices.size() <= std::numeric_limits<unsigned short>::max());

    Renderable renderable{ static_cast<unsigned short>(indices.size()) };
    glBindVertexArray(renderable.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(ShaderVertexAttrib::Position.location(), 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(ShaderVertexAttrib::Position.location());
    glVertexAttribPointer(ShaderVertexAttrib::Color.location(), 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(ShaderVertexAttrib::Color.location());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
    return renderable;
}

}  // namespace firstgame::render