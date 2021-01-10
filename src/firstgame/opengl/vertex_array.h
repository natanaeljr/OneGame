#ifndef FIRSTGAME_OPENGL_VERTEX_ARRAYS_H_
#define FIRSTGAME_OPENGL_VERTEX_ARRAYS_H_

#include <utility>

namespace firstgame::opengl {

/// Representation of a opengl vertex array generated with glGenVertexArrays
struct VertexArray {
    /// Create and generate a vertex array object
    VertexArray();

    /// Delete buffer if non-zero
    ~VertexArray();

    /// For creating a null VertexArray
    struct Null {
    };
    /// Create a non-initialized VertexArray
    VertexArray(Null) noexcept : id(0) {}

    /// Implicit cast to the buffer ID
    operator unsigned int() const { return id; }

    /// Move constructor
    VertexArray(VertexArray&& other) noexcept : id(std::exchange(other.id, 0)) {}

    /// Move Assignment
    VertexArray& operator=(VertexArray&& other) noexcept
    {
        id = std::exchange(other.id, 0);
        return *this;
    }

    /// Deleted Copy constructor/assignment
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

   private:
    unsigned int id;
};

}  // namespace firstgame::opengl

#endif  // FIRSTGAME_OPENGL_VERTEX_ARRAYS_H_
