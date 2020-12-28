#ifndef FIRSTGAME_RENDERER_SHADER_VARIABLE_H_
#define FIRSTGAME_RENDERER_SHADER_VARIABLE_H_

#include "firstgame/system/log.h"

namespace firstgame::renderer {

/// ShaderVariable is meant to provide a way to generalize the shaders inputs, so that the code can
/// perform the same tasks on any shader that follows this shader variable patters, discarding the
/// need for hard-coded Shader information for each shader program written.
struct ShaderVariable final {
    /// List of all supported variables for using in Shaders
    enum class Enum : unsigned char {
        InPosition = 0,
        InTexCoord,
        InColor,
        UniformColor,
        UniformModel,
        UniformView,
        UniformProjection,
        UniformModelView,
        UniformViewProjection,
        UniformModelViewProjection,
        UniformTexture1,
        UniformTexture2,
    };
    /// Total number of Variables
    static constexpr size_t Count = (static_cast<size_t>(Enum::UniformTexture2) + 1u);

    /// Convenience declarations in object form for easy use
    static const ShaderVariable InPosition;
    static const ShaderVariable InTexCoord;
    static const ShaderVariable InColor;
    static const ShaderVariable UniformColor;
    static const ShaderVariable UniformModel;
    static const ShaderVariable UniformView;
    static const ShaderVariable UniformProjection;
    static const ShaderVariable UniformModelView;
    static const ShaderVariable UniformViewProjection;
    static const ShaderVariable UniformModelViewProjection;
    static const ShaderVariable UniformTexture1;
    static const ShaderVariable UniformTexture2;

    /// Get the standardized variable name
    [[nodiscard]] inline constexpr const char* string() const
    {
        switch (enum_) {
            case Enum::InPosition: return "inPosition";
            case Enum::InTexCoord: return "inTexCoord";
            case Enum::InColor: return "inColor";
            case Enum::UniformColor: return "uColor";
            case Enum::UniformModel: return "uModel";
            case Enum::UniformView: return "uView";
            case Enum::UniformProjection: return "uProjection";
            case Enum::UniformModelView: return "uModelView";
            case Enum::UniformViewProjection: return "uViewProjection";
            case Enum::UniformModelViewProjection: return "uModelViewProjection";
            case Enum::UniformTexture1: return "uTexture1";
            case Enum::UniformTexture2: return "uTexture2";
        }
        ASSERT_MSG(0, "Invalid shader variable {}", enum_);
        return nullptr;
    }

    /// Implicit construct from the enum form
    constexpr ShaderVariable(Enum e) noexcept : enum_(e) {}
    /// Implicit cast to the enum form
    constexpr operator Enum() const noexcept { return enum_; }
    /// Get the underlying enumerator
    [[nodiscard]] constexpr Enum enumerator() const { return enum_; }
    /// Get the index number of the enumerator
    [[nodiscard]] constexpr auto index() const
    {
        return static_cast<std::underlying_type_t<Enum>>(enum_);
    }
    /// Logical operators, for more, use the Enum type
    constexpr bool operator==(ShaderVariable other) const { return this->enum_ == other.enum_; }
    constexpr bool operator!=(ShaderVariable other) const { return this->enum_ != other.enum_; }

   private:
    Enum enum_;
};

// clang-format off
constexpr ShaderVariable ShaderVariable::InPosition{ ShaderVariable::Enum::InPosition };
constexpr ShaderVariable ShaderVariable::InTexCoord{ ShaderVariable::Enum::InTexCoord };
constexpr ShaderVariable ShaderVariable::InColor{ ShaderVariable::Enum::InColor };
constexpr ShaderVariable ShaderVariable::UniformColor{ ShaderVariable::Enum::UniformColor };
constexpr ShaderVariable ShaderVariable::UniformModel{ ShaderVariable::Enum::UniformModel };
constexpr ShaderVariable ShaderVariable::UniformView{ ShaderVariable::Enum::UniformView };
constexpr ShaderVariable ShaderVariable::UniformProjection{ ShaderVariable::Enum::UniformProjection };
constexpr ShaderVariable ShaderVariable::UniformModelView{ ShaderVariable::Enum::UniformModelView };
constexpr ShaderVariable ShaderVariable::UniformViewProjection{ ShaderVariable::Enum::UniformViewProjection };
constexpr ShaderVariable ShaderVariable::UniformModelViewProjection{ ShaderVariable::Enum::UniformModelViewProjection };
constexpr ShaderVariable ShaderVariable::UniformTexture1{ ShaderVariable::Enum::UniformTexture1 };
constexpr ShaderVariable ShaderVariable::UniformTexture2{ ShaderVariable::Enum::UniformTexture2 };
// clang-format on

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_SHADER_VARIABLE_H_
