#ifndef FIRSTGAME_RENDERER_COMPONENT_H_
#define FIRSTGAME_RENDERER_COMPONENT_H_

#include <utility>
#include "firstgame/opengl/gl/types.h"

namespace firstgame::renderer {

/// Render Component contains GPU-uploaded data, ready to be rendered.
struct RenderComponent final {
    GLuint vao{};
    GLuint vbo{};
    GLuint ebo{};
    unsigned short elements{};

    /// Generate the buffer objects on GPU
    explicit RenderComponent(unsigned short elements);

    /// Create from existing objects
    RenderComponent(GLuint vao, GLuint vbo, GLuint ebo, unsigned short count)
        : vao(vao), vbo(vbo), ebo(ebo), elements(count)
    {
    }

    /// Delete buffer objects if valid
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
          elements(std::exchange(other.elements, 0))
    {
    }

    /// Move Assignment
    RenderComponent& operator=(RenderComponent&& other) noexcept
    {
        vao = std::exchange(other.vao, 0);
        vbo = std::exchange(other.vbo, 0);
        ebo = std::exchange(other.ebo, 0);
        elements = std::exchange(other.elements, 0);
        return *this;
    }

    /// Deleted Copy constructor/assignment
    RenderComponent(const RenderComponent&) = delete;
    RenderComponent& operator=(const RenderComponent&) = delete;

    /// Transform to std::tie
    [[nodiscard]] inline auto tie() const { return std::tie(vao, vbo, ebo, elements); }

    /// Equality operator
    bool operator==(const RenderComponent& other) const { return this->tie() == other.tie(); }
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_COMPONENT_H_
