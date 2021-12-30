#include "painter.h"

#include <gsl/span>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

namespace firstgame::render {

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

struct Instance {
    glm::mat4 model;
};

Renderable GenerateRenderable(gsl::span<const Vertex> vertices, gsl::span<const unsigned short> indices);

RenderableInstanced GenerateRenderableInstanced(gsl::span<const Vertex> vertices,
                                                gsl::span<const unsigned short> indices,
                                                gsl::span<const Instance> instances);

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
        { .position = { -1.0f, -1.0f, +1.0f }, .color = { 1.0f, 1.0f, 0.0f, 1.0f } },  // [0] A front
        { .position = { -1.0f, +1.0f, +1.0f }, .color = { 1.0f, 0.0f, 1.0f, 1.0f } },  // [1] B front
        { .position = { +1.0f, -1.0f, +1.0f }, .color = { 1.0f, 0.0f, 0.0f, 1.0f } },  // [2] C front
        { .position = { +1.0f, +1.0f, +1.0f }, .color = { 0.0f, 0.0f, 1.0f, 1.0f } },  // [3] D front
        { .position = { -1.0f, -1.0f, -1.0f }, .color = { 0.5f, 0.0f, 0.5f, 1.0f } },  // [4] A back
        { .position = { -1.0f, +1.0f, -1.0f }, .color = { 0.0f, 1.0f, 0.0f, 1.0f } },  // [5] B back
        { .position = { +1.0f, -1.0f, -1.0f }, .color = { 0.5f, 0.5f, 0.0f, 1.0f } },  // [6] C back
        { .position = { +1.0f, +1.0f, -1.0f }, .color = { 0.0f, 0.5f, 0.5f, 1.0f } },  // [7] D back
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

RenderableInstanced GenerateCubeInstanced(unsigned int rows, unsigned int cols)
{
    static constexpr Vertex vertices[] = {
        // clang-format off
        { .position = { -1.0f, -1.0f, +1.0f }, .color = { 1.0f, 1.0f, 0.0f, 1.0f } },  // [0] A front
        { .position = { -1.0f, +1.0f, +1.0f }, .color = { 1.0f, 0.0f, 1.0f, 1.0f } },  // [1] B front
        { .position = { +1.0f, -1.0f, +1.0f }, .color = { 1.0f, 0.0f, 0.0f, 1.0f } },  // [2] C front
        { .position = { +1.0f, +1.0f, +1.0f }, .color = { 0.0f, 0.0f, 1.0f, 1.0f } },  // [3] D front
        { .position = { -1.0f, -1.0f, -1.0f }, .color = { 0.5f, 0.0f, 0.5f, 1.0f } },  // [4] A back
        { .position = { -1.0f, +1.0f, -1.0f }, .color = { 0.0f, 1.0f, 0.0f, 1.0f } },  // [5] B back
        { .position = { +1.0f, -1.0f, -1.0f }, .color = { 0.5f, 0.5f, 0.0f, 1.0f } },  // [6] C back
        { .position = { +1.0f, +1.0f, -1.0f }, .color = { 0.0f, 0.5f, 0.5f, 1.0f } },  // [7] D back
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
    std::vector<Instance> instances{ rows * cols };
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            const auto index = (i * cols + j);
            instances[index] = {
                .model =
                    [i, j] {
                        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(float(i), -3.0f, float(j)));
                        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
                        return translation * scale;
                    }(),
            };
        }
    }
    return GenerateRenderableInstanced(vertices, indices, instances);
}

/**************************************************************************************************/

Renderable GenerateRenderable(gsl::span<const Vertex> vertices, gsl::span<const unsigned short> indices)
{
    ASSERT(indices.size() <= std::numeric_limits<unsigned short>::max());

    Renderable renderable{ static_cast<unsigned short>(indices.size()) };
    glBindVertexArray(renderable.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(ShaderVertexAttrib::Position.location(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(ShaderVertexAttrib::Position.location());
    glVertexAttribPointer(ShaderVertexAttrib::Color.location(), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(ShaderVertexAttrib::Color.location());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
    return renderable;
}

/**************************************************************************************************/

RenderableInstanced GenerateRenderableInstanced(gsl::span<const Vertex> vertices,
                                                gsl::span<const unsigned short> indices,
                                                gsl::span<const Instance> instances)
{
    ASSERT(indices.size() <= std::numeric_limits<unsigned short>::max());
    ASSERT(instances.size() <= std::numeric_limits<unsigned int>::max());

    RenderableInstanced renderable{ static_cast<unsigned short>(indices.size()),
                                    static_cast<unsigned int>(instances.size()) };

    glBindVertexArray(renderable.vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(ShaderVertexAttrib::Position.location());
    glVertexAttribPointer(ShaderVertexAttrib::Position.location(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(ShaderVertexAttrib::Color.location());
    glVertexAttribPointer(ShaderVertexAttrib::Color.location(), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, renderable.ibo);
    glBufferData(GL_ARRAY_BUFFER, instances.size_bytes(), instances.data(), GL_STATIC_DRAW);

    for (unsigned index : { 0, 1, 2, 3 }) {
        const unsigned location = ShaderVertexAttrib::Model0.location() + index;
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                              (void*) (index * sizeof(glm::mat4::col_type)));
        glVertexAttribDivisor(location, 1);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

    return renderable;
}

}  // namespace firstgame::render