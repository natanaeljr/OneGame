#ifndef FIRSTGAME_RENDERER_SHADER_UNIFORM_H_
#define FIRSTGAME_RENDERER_SHADER_UNIFORM_H_

#include "firstgame/system/log.h"

namespace firstgame::renderer {

/// ShaderUniform is meant to provide a way to generalize the shaders inputs, so that the code can
/// perform the same tasks on any shader that follows this shader variable patters, discarding the
/// need for hard-coded Shader information for each shader program written.
struct ShaderUniform {
    /// List of all supported uniform variables for using in Shaders
    enum class Enum {
        Color = 0,
        Model = 1,
        View = 2,
        Projection = 3,
        ModelView = 4,
        ViewProjection = 5,
        ModelViewProjection = 6,
        Texture0 = 10,
        Texture1 = 11,
        Texture2 = 12,
    };

    /// Declarations in object form for convenience
    static const ShaderUniform Color;
    static const ShaderUniform Model;
    static const ShaderUniform View;
    static const ShaderUniform Projection;
    static const ShaderUniform ModelView;
    static const ShaderUniform ViewProjection;
    static const ShaderUniform ModelViewProjection;
    static const ShaderUniform Texture0;
    static const ShaderUniform Texture1;
    static const ShaderUniform Texture2;

    /// Get the standardized variable name
    [[nodiscard]] inline constexpr const char* string() const
    {
        switch (enum_) {
            case Enum::Color: return "uColor";
            case Enum::Model: return "uModel";
            case Enum::View: return "uView";
            case Enum::Projection: return "uProjection";
            case Enum::ModelView: return "uModelView";
            case Enum::ViewProjection: return "uViewProjection";
            case Enum::ModelViewProjection: return "uModelViewProjection";
            case Enum::Texture0: return "uTexture0";
            case Enum::Texture1: return "uTexture1";
            case Enum::Texture2: return "uTexture2";
        }
        ASSERT_MSG(0, "Invalid shader uniform {}", enum_);
        return nullptr;
    }

    /// Implicit construct from the enum form
    constexpr ShaderUniform(Enum e) noexcept : enum_(e) {}
    /// Implicit cast to the enum form
    constexpr operator Enum() const noexcept { return enum_; }
    /// Get the underlying enumerator
    [[nodiscard]] constexpr Enum enumerator() const { return enum_; }
    /// Get the location/index of the uniform
    [[nodiscard]] constexpr auto location() const
    {
        return static_cast<std::underlying_type_t<Enum>>(enum_);
    }
    /// Logical operators, for more, use the Enum type
    constexpr bool operator==(ShaderUniform other) const { return this->enum_ == other.enum_; }
    constexpr bool operator!=(ShaderUniform other) const { return this->enum_ != other.enum_; }

   private:
    Enum enum_;
};

// clang-format off
constexpr ShaderUniform ShaderUniform::Color{ ShaderUniform::Enum::Color };
constexpr ShaderUniform ShaderUniform::Model{ ShaderUniform::Enum::Model };
constexpr ShaderUniform ShaderUniform::View{ ShaderUniform::Enum::View };
constexpr ShaderUniform ShaderUniform::Projection{ ShaderUniform::Enum::Projection };
constexpr ShaderUniform ShaderUniform::ModelView{ ShaderUniform::Enum::ModelView };
constexpr ShaderUniform ShaderUniform::ViewProjection{ ShaderUniform::Enum::ViewProjection };
constexpr ShaderUniform ShaderUniform::ModelViewProjection{ ShaderUniform::Enum::ModelViewProjection };
constexpr ShaderUniform ShaderUniform::Texture0{ ShaderUniform::Enum::Texture0 };
constexpr ShaderUniform ShaderUniform::Texture1{ ShaderUniform::Enum::Texture1 };
constexpr ShaderUniform ShaderUniform::Texture2{ ShaderUniform::Enum::Texture2 };
// clang-format on

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_SHADER_UNIFORM_H_
