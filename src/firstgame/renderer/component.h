#ifndef FIRSTGAME_RENDERER_COMPONENT_H_
#define FIRSTGAME_RENDERER_COMPONENT_H_

#include <tuple>
#include <utility>

namespace firstgame::renderer {

/// Render Component contains GPU-uploaded data, ready to be rendered.
struct RenderComponent final {
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int ebo{};
    unsigned short num_vertices{};

    /// Create and generate the buffer objects on GPU
    explicit RenderComponent(unsigned short elements);

    /// Create from existing objects
    RenderComponent(unsigned int vao, unsigned int vbo, unsigned int ebo,
                    unsigned short num_vertices)
        : vao(vao), vbo(vbo), ebo(ebo), num_vertices(num_vertices)
    {
    }

    /// Delete buffer objects if non-zero
    ~RenderComponent();

    /// For creating a null RenderComponent
    struct Null {
    };

    /// Create a non-initialized RenderComponent
    RenderComponent(Null) noexcept {}

    /// Move constructor
    RenderComponent(RenderComponent&& other) noexcept
        : vao(std::exchange(other.vao, 0)),
          vbo(std::exchange(other.vbo, 0)),
          ebo(std::exchange(other.ebo, 0)),
          num_vertices(std::exchange(other.num_vertices, 0))
    {
    }

    /// Move Assignment
    RenderComponent& operator=(RenderComponent&& other) noexcept
    {
        vao = std::exchange(other.vao, 0);
        vbo = std::exchange(other.vbo, 0);
        ebo = std::exchange(other.ebo, 0);
        num_vertices = std::exchange(other.num_vertices, 0);
        return *this;
    }

    /// Deleted Copy constructor/assignment
    RenderComponent(const RenderComponent&) = delete;
    RenderComponent& operator=(const RenderComponent&) = delete;

    /// Transform to std::tie
    [[nodiscard]] inline auto tie() const { return std::tie(vao, vbo, ebo, num_vertices); }

    /// Equality operator
    bool operator==(const RenderComponent& other) const { return this->tie() == other.tie(); }
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_COMPONENT_H_
