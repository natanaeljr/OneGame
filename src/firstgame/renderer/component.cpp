#include "component.h"

#include "firstgame/opengl/gl/functions.h"

namespace firstgame::renderer {

RenderComponent::RenderComponent(unsigned short elements) : elements(elements)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

RenderComponent::~RenderComponent()
{
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (ebo)
        glDeleteBuffers(1, &ebo);
}

}  // namespace firstgame::renderer
