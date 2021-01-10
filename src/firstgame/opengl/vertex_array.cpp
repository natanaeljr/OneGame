#include "vertex_array.h"

#include "gl/functions.h"

namespace firstgame::opengl {

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
    if (id) {
        glDeleteVertexArrays(1, &id);
    }
}

}  // namespace firstgame::opengl
