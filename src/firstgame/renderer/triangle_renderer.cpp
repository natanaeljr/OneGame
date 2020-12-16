#include "triangle_renderer.h"

#include "firstgame/system/log.h"

namespace firstgame::renderer {

/**************************************************************************************************/

TriangleRenderer::TriangleRenderer()
{
    TRACE("Created TriangleRenderer");
    // TODO: Create VAO, VBO, EBO
}

/**************************************************************************************************/

TriangleRenderer::~TriangleRenderer()
{
    // TODO: Delete VAO, VBO, EBO
    TRACE("Destroyed TriangleRenderer");
}

/**************************************************************************************************/

void TriangleRenderer::Render()
{
    // TODO: Draw Triangle
}

}  // namespace firstgame::renderer
