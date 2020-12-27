#ifndef FIRSTGAME_RENDERER_HELPER_H_
#define FIRSTGAME_RENDERER_HELPER_H_

#include <gsl/span>
#include "component.h"
#include "firstgame/opengl/gl/types.h"

namespace firstgame::renderer {

RenderComponent GenerateQuad();

RenderComponent GenerateRenderComponent(gsl::span<const GLfloat> vertices,
                                        gsl::span<const GLushort> indices);

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_HELPER_H_
