#ifndef FIRSTGAME_SHADER_MODEL_H_
#define FIRSTGAME_SHADER_MODEL_H_

#include "firstgame/system/log.h"

namespace firstgame::renderer {

/// ShaderVariable is meant to provide a way to generalize the shaders inputs, so that the code can
/// perform the same tasks on multiple shaders that follow this shader variable patters.

/// List of all supported variables for using in Shaders
struct ShaderVariable final {
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
    static constexpr size_t Count = (static_cast<size_t>(Enum::UniformTexture2) + 1u);

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

    [[nodiscard]] inline constexpr const char* string() const
    {
        switch (enum_) {
            case Enum::InPosition:
                return "inPosition";
            case Enum::InTexCoord:
                return "inTexCoord";
            case Enum::InColor:
                return "inColor";
            case Enum::UniformColor:
                return "uColor";
            case Enum::UniformModel:
                return "uModel";
            case Enum::UniformView:
                return "uView";
            case Enum::UniformProjection:
                return "uProjection";
            case Enum::UniformModelView:
                return "uModelView";
            case Enum::UniformViewProjection:
                return "uViewProjection";
            case Enum::UniformModelViewProjection:
                return "uModelViewProjection";
            case Enum::UniformTexture1:
                return "uTexture1";
            case Enum::UniformTexture2:
                return "uTexture2";
        }
        ASSERT_MSG(0, "Invalid shader variable {}", enum_);
        return nullptr;
    }

    constexpr ShaderVariable(Enum e) noexcept : enum_(e) {}
    constexpr operator Enum() const noexcept { return enum_; }
    [[nodiscard]] constexpr Enum enumerator() const { return enum_; }
    [[nodiscard]] constexpr auto index() const
    {
        return static_cast<std::underlying_type_t<Enum>>(enum_);
    }
    constexpr bool operator==(ShaderVariable other) const { return this->enum_ == other.enum_; }
    constexpr bool operator!=(ShaderVariable other) const { return this->enum_ == other.enum_; }

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
