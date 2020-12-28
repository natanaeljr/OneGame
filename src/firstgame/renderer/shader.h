#ifndef FIRSTGAME_RENDERER_SHADER_H_
#define FIRSTGAME_RENDERER_SHADER_H_

#include "firstgame/opengl/shader.h"
#include "firstgame/system/log.h"
#include "shader_variable.h"
#include <array>

namespace firstgame::renderer {

/// Generalized interface for operating on a Shader
class Shader {
   public:
    opengl::Shader* operator->() { return &shader_; }
    const opengl::Shader* operator->() const { return &shader_; }
    
    // TODO: Maybe do a ShaderLayout for locations?

    [[nodiscard]] bool HasLocation(ShaderVariable variable) const
    {
        return GetLocation(variable) != -1;
    }
    [[nodiscard]] int GetLocation(ShaderVariable variable) const
    {
        return locations_[variable.index()];
    }
    [[nodiscard]] int GetLocationAssert(ShaderVariable variable) const
    {
        ASSERT_MSG(HasLocation(variable),
                   "ShaderVariable{{\"{}\"}} location not found in shader \"{}\" [{}]",
                   variable.string(), name_.data(), shader_.program);
        return GetLocation(variable);
    }

   private:
    opengl::Shader shader_{};
    std::array<char, 20> name_{};
    std::array<int, ShaderVariable::Count> locations_{};
};

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_SHADER_H_
