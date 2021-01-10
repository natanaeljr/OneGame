#ifndef FIRSTGAME_RENDER_PAINTER_H_
#define FIRSTGAME_RENDER_PAINTER_H_

#include "renderable.h"
#include "renderable_instanced.h"

namespace firstgame::render {

Renderable GenerateQuad();

Renderable GenerateCube();

RenderableInstanced GenerateCubeInstanced(unsigned int rows, unsigned int cols);

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_PAINTER_H_
