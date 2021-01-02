#ifndef FIRSTGAME_RENDER_RENDERABLE_H_
#define FIRSTGAME_RENDER_RENDERABLE_H_

#include <tuple>
#include <utility>

namespace firstgame::render {

/// Renderable Component contains GPU-uploaded data, ready to be rendered.
struct Renderable final {
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int ebo{};
    unsigned short num_vertices{};

    /// Create and generate the buffer objects on GPU
    explicit Renderable(unsigned short elements);

    /// Create from existing objects
    Renderable(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned short num_vertices)
        : vao(vao), vbo(vbo), ebo(ebo), num_vertices(num_vertices)
    {
    }

    /// Delete buffer objects if non-zero
    ~Renderable();

    /// For creating a null Renderable
    struct Null {
    };

    /// Create a non-initialized Renderable
    Renderable(Null) noexcept {}

    /// Move constructor
    Renderable(Renderable&& other) noexcept
        : vao(std::exchange(other.vao, 0)),
          vbo(std::exchange(other.vbo, 0)),
          ebo(std::exchange(other.ebo, 0)),
          num_vertices(std::exchange(other.num_vertices, 0))
    {
    }

    /// Move Assignment
    Renderable& operator=(Renderable&& other) noexcept
    {
        vao = std::exchange(other.vao, 0);
        vbo = std::exchange(other.vbo, 0);
        ebo = std::exchange(other.ebo, 0);
        num_vertices = std::exchange(other.num_vertices, 0);
        return *this;
    }

    /// Deleted Copy constructor/assignment
    Renderable(const Renderable&) = delete;
    Renderable& operator=(const Renderable&) = delete;

    /// Transform to std::tie
    [[nodiscard]] inline auto tie() const { return std::tie(vao, vbo, ebo, num_vertices); }

    /// Equality operator
    bool operator==(const Renderable& other) const { return this->tie() == other.tie(); }
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_RENDERABLE_H_
