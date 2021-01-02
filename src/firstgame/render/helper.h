#ifndef FIRSTGAME_RENDER_HELPER_H_
#define FIRSTGAME_RENDER_HELPER_H_

#include <gsl/span>
#include "renderable.h"

namespace firstgame::render {

Renderable GenerateQuad();

Renderable GenerateRenderable(gsl::span<const float> vertices,
                                        gsl::span<const unsigned short> indices);

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_HELPER_H_
