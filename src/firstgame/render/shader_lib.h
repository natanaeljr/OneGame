#ifndef FIRSTGAME_RENDER_SHADERS_H_
#define FIRSTGAME_RENDER_SHADERS_H_

#include "firstgame/util/currenton.h"
#include "firstgame/util/scoped.h"
#include "firstgame/opengl/shader.h"

namespace firstgame::render {

/// Enumeration of shaders supported by the engine.
enum class MyShader {
    /// Simplest Shader for rendering colored polygons.
    /// attribs: vec3 position, vec4 color.
    /// uniforms: mat4 model, mat4 view, mat4 projection.
    SIMPLE,
    /// Instance Shader for rendering colored polygons in batch.
    /// attribs: vec3 position, vec4 color.
    /// uniforms: mat4 model, mat4 view, mat4 projection.
    SIMPLE_INSTANCE,
    /// Total number of shaders.
    /// Must be last.
    COUNT,
};

/// ShadersLibrary contains all shaders used throughout the engine.
/// Since it is a Currenton, anyone can retrieve a shader object with get().
/// Remember to load the shaders before they are queried.
/// Also for current() to work, the global ShaderLibrary needs to be instanced somewhere in the program.
class ShaderLibrary final : public util::Currenton<ShaderLibrary> {
   public:
    ShaderLibrary() = default;
    ShaderLibrary(const ShaderLibrary&) = delete;
    ShaderLibrary& operator=(const ShaderLibrary&) = delete;

   public:
    /// Get shader from library
    [[nodiscard]] opengl::GLShader& get(MyShader my_shader);

    /// Load shader into library
    template<MyShader>
    void load();

    /// Unload shader from library
    void unload(MyShader my_shader);

   private:
    /// Library of shaders
    util::Scoped<opengl::GLShader> shaders_[static_cast<size_t>(MyShader::COUNT)];
};

}  // namespace firstgame::render
#endif  // FIRSTGAME_RENDER_SHADERS_H_
