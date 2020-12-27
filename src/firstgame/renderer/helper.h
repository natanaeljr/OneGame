#ifndef FIRSTGAME_RENDERER_HELPER_H_
#define FIRSTGAME_RENDERER_HELPER_H_

#include <gsl/span>
#include "component.h"

namespace firstgame::renderer {

RenderComponent GenerateQuad();

RenderComponent GenerateRenderComponent(gsl::span<const float> vertices,
                                        gsl::span<const unsigned short> indices);

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_HELPER_H_
