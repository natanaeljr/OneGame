#ifndef FIRSTGAME_RENDERER_SHADER_ATTRIB_H_
#define FIRSTGAME_RENDERER_SHADER_ATTRIB_H_

#include "firstgame/system/log.h"

namespace firstgame::renderer {

/// ShaderAttrib is meant to provide a way to generalize the shaders inputs, so that the code can
/// perform the same tasks on any shader that follows this shader variable patters, discarding the
/// need for hard-coded Shader information for each shader program written.
struct ShaderAttrib {
    /// List of all supported vertex attributes for using in Shaders
    enum class Enum {
        Position = 0,
        TexCoord = 1,
        Color = 2,
    };

    /// Declarations in object form for convenience
    static const ShaderAttrib Position;
    static const ShaderAttrib TexCoord;
    static const ShaderAttrib Color;

    /// Get the standardized variable name
    [[nodiscard]] inline constexpr const char* string() const
    {
        switch (enum_) {
            case Enum::Position: return "inPosition";
            case Enum::TexCoord: return "inTexCoord";
            case Enum::Color: return "inColor";
        }
        ASSERT_MSG(0, "Invalid shader attribute {}", enum_);
        return nullptr;
    }

    /// Implicit construct from the enum form
    constexpr ShaderAttrib(Enum e) noexcept : enum_(e) {}
    /// Implicit cast to the enum form
    constexpr operator Enum() const noexcept { return enum_; }
    /// Get the underlying enumerator
    [[nodiscard]] constexpr Enum enumerator() const { return enum_; }
    /// Get the location/index of the attribute
    [[nodiscard]] constexpr auto location() const
    {
        return static_cast<std::underlying_type_t<Enum>>(enum_);
    }
    /// Logical operators, for more, use the Enum type
    constexpr bool operator==(ShaderAttrib other) const { return this->enum_ == other.enum_; }
    constexpr bool operator!=(ShaderAttrib other) const { return this->enum_ != other.enum_; }

   private:
    Enum enum_;
};

constexpr ShaderAttrib ShaderAttrib::Position{ ShaderAttrib::Enum::Position };
constexpr ShaderAttrib ShaderAttrib::TexCoord{ ShaderAttrib::Enum::TexCoord };
constexpr ShaderAttrib ShaderAttrib::Color{ ShaderAttrib::Enum::Color };

}  // namespace firstgame::renderer

#endif  // FIRSTGAME_RENDERER_SHADER_ATTRIB_H_
