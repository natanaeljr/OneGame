#ifndef FIRSTGAME_OPENGL_BUFFER_H_
#define FIRSTGAME_OPENGL_BUFFER_H_

#include <utility>
#include "gl/functions.h"

namespace firstgame::opengl {

/// Representation of a opengl buffer generated with glGenBuffers
struct Buffer {
    /// Create and generate the buffer object
    Buffer() { glGenBuffers(1, &id); }

    /// Delete buffer if non-zero
    ~Buffer()
    {
        if (id)
            glDeleteBuffers(1, &id);
    }

    /// For creating a null Buffer
    struct Null {
    };
    /// Create a non-initialized Buffer
    Buffer(Null) noexcept : id(0) {}

    /// Implicit cast to the buffer ID
    operator GLuint() const { return id; }

    /// Move constructor
    Buffer(Buffer&& other) noexcept : id(std::exchange(other.id, 0)) {}

    /// Move Assignment
    Buffer& operator=(Buffer&& other) noexcept
    {
        id = std::exchange(other.id, 0);
        return *this;
    }

    /// Deleted Copy constructor/assignment
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

   private:
    GLuint id;
};

}  // namespace firstgame::opengl

#endif  // FIRSTGAME_OPENGL_BUFFER_H_
