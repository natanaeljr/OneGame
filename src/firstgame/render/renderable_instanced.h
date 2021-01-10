#ifndef FIRSTGAME_RENDER_RENDERABLE_INSTANCED_H_
#define FIRSTGAME_RENDER_RENDERABLE_INSTANCED_H_

#include <tuple>
#include <utility>

#include "firstgame/opengl/vertex_array.h"
#include "firstgame/opengl/buffer.h"

namespace firstgame::render {

/// RenderableInstanced Component contains GPU-uploaded data, ready to be multi-instance rendered.
struct RenderableInstanced final {
    opengl::VertexArray vao{};  ///< vertex array
    opengl::Buffer vbo{};       ///< vertex buffer
    opengl::Buffer ibo{};       ///< instance buffer
    opengl::Buffer ebo{};       ///< element buffer
    unsigned short num_vertices{};
    unsigned int num_instances{};

    /// Create and generate the buffer objects on GPU
    explicit RenderableInstanced(unsigned short num_vertices) : num_vertices(num_vertices) {}

    /// Create from existing objects
    RenderableInstanced(opengl::VertexArray&& vao, opengl::Buffer&& vbo, opengl::Buffer&& ebo,
                        unsigned short num_vertices)
        : vao(std::move(vao)), vbo(std::move(vbo)), ebo(std::move(ebo)), num_vertices(num_vertices)
    {
    }

    /// For creating a null RenderableInstanced
    struct Null {
    };

    /// Create a null/invalid RenderableInstanced
    RenderableInstanced(Null) noexcept
        : vao(opengl::VertexArray::Null{}),
          vbo(opengl::Buffer::Null{}),
          ibo(opengl::Buffer::Null{}),
          ebo(opengl::Buffer::Null{}),
          num_vertices(0),
          num_instances(0)
    {
    }

    /// Default Move constructor/assignment
    RenderableInstanced(RenderableInstanced&& other) noexcept = default;
    RenderableInstanced& operator=(RenderableInstanced&& other) noexcept = default;

    /// Deleted Copy constructor/assignment
    RenderableInstanced(const RenderableInstanced&) = delete;
    RenderableInstanced& operator=(const RenderableInstanced&) = delete;

    /// Transform to std::tie
    [[nodiscard]] inline auto tie() const { return std::tie(vao, vbo, ebo, num_vertices); }

    /// Equality operator
    bool operator==(const RenderableInstanced& other) const { return this->tie() == other.tie(); }
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_RENDERABLE_INSTANCED_H_
