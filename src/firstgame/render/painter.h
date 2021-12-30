#ifndef FIRSTGAME_RENDER_PAINTER_H_
#define FIRSTGAME_RENDER_PAINTER_H_

#include "renderable.h"
#include "renderable_instanced.h"
#include "firstgame/opengl/shader.h"

namespace firstgame::render {

Renderable GenerateQuad(const opengl::GLShader& shader);

Renderable GenerateCube(const opengl::GLShader& shader);

RenderableInstanced GenerateCubeInstanced(const opengl::GLShader& shader, unsigned int rows, unsigned int cols);

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_PAINTER_H_
