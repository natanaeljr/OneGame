#ifndef FIRSTGAME_RENDER_RENDERABLE_H_
#define FIRSTGAME_RENDER_RENDERABLE_H_

#include <tuple>
#include <utility>

#include "firstgame/opengl/vertex_array.h"
#include "firstgame/opengl/buffer.h"

namespace firstgame::render {

/// Renderable Component contains GPU-uploaded data, ready to be rendered.
struct Renderable final {
    opengl::VertexArray vao{};
    opengl::Buffer vbo{};
    opengl::Buffer ebo{};
    unsigned short num_indices{};

    /// Create and generate the buffer objects on GPU
    explicit Renderable(unsigned short num_indices) : num_indices(num_indices) {}

    /// Create from existing objects
    Renderable(opengl::VertexArray&& vao, opengl::Buffer&& vbo, opengl::Buffer&& ebo,
               unsigned short num_vertices)
        : vao(std::move(vao)), vbo(std::move(vbo)), ebo(std::move(ebo)), num_indices(num_vertices)
    {
    }

    /// For creating a null Renderable
    struct Null {
    };

    /// Create a null/invalid Renderable
    Renderable(Null) noexcept
        : vao(opengl::VertexArray::Null{}),
          vbo(opengl::Buffer::Null{}),
          ebo(opengl::Buffer::Null{}),
          num_indices(0)
    {
    }

    /// Default Move constructor/assignment
    Renderable(Renderable&& other) noexcept = default;
    Renderable& operator=(Renderable&& other) noexcept = default;

    /// Deleted Copy constructor/assignment
    Renderable(const Renderable&) = delete;
    Renderable& operator=(const Renderable&) = delete;

    /// Transform to std::tie
    [[nodiscard]] inline auto tie() const { return std::tie(vao, vbo, ebo, num_indices); }

    /// Equality operator
    bool operator==(const Renderable& other) const { return this->tie() == other.tie(); }
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_RENDERABLE_H_
