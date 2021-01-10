#include "buffer.h"

#include "gl/functions.h"

namespace firstgame::opengl {

Buffer::Buffer()
{
    glGenBuffers(1, &id);
}

Buffer::~Buffer()
{
    if (id) {
        glDeleteBuffers(1, &id);
    }
}

}  // namespace firstgame::opengl
