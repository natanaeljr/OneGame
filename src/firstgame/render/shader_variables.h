#ifndef FIRSTGAME_RENDER_SHADER_VARIABLES_H_
#define FIRSTGAME_RENDER_SHADER_VARIABLES_H_

namespace firstgame::render {

/// ShaderVertexAttrib is meant to provide a way to generalize the shaders inputs, so that the code
/// can perform the same tasks on any shader that follows this shader variable patters, discarding
/// the need for hard-coded Shader information for each shader program written.
struct ShaderVertexAttrib {
    /// List of all supported vertex attributes for using in Shaders
    enum class Enum : unsigned {
        Position = 0,
        TexCoord = 1,
        Color = 2,
        Model0 = 8,
        Model1 = 9,
        Model2 = 10,
        Model3 = 11,
    };

    /// Declarations in object form for convenience
    static const ShaderVertexAttrib Position;
    static const ShaderVertexAttrib TexCoord;
    static const ShaderVertexAttrib Color;
    static const ShaderVertexAttrib Model0;
    static const ShaderVertexAttrib Model1;
    static const ShaderVertexAttrib Model2;
    static const ShaderVertexAttrib Model3;

    /// Get the standardized variable name
    [[nodiscard]] inline constexpr const char* string() const
    {
        switch (enum_) {
            case Enum::Position: return "inPosition";
            case Enum::TexCoord: return "inTexCoord";
            case Enum::Color: return "inColor";
            case Enum::Model0:
            case Enum::Model1:
            case Enum::Model2:
            case Enum::Model3: return "inModel";
        }
        return nullptr;
    }

    /// Implicit construct from the enum form
    constexpr ShaderVertexAttrib(Enum e) noexcept : enum_(e) {}
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
    constexpr bool operator==(ShaderVertexAttrib other) const { return this->enum_ == other.enum_; }
    constexpr bool operator!=(ShaderVertexAttrib other) const { return this->enum_ != other.enum_; }

   private:
    Enum enum_;
};

/// ShaderUniform is meant to provide a way to generalize the shaders inputs, so that the code can
/// perform the same tasks on any shader that follows this shader variable patters, discarding the
/// need for hard-coded Shader information for each shader program written.
struct ShaderUniform {
    /// List of all supported uniform variables for using in Shaders
    enum class Enum : unsigned {
        Color = 0,
        Model = 1,
        View = 2,
        Projection = 3,
        Texture0 = 10,
        Texture1 = 11,
        Texture2 = 12,
    };

    /// Declarations in object form for convenience
    static const ShaderUniform Color;
    static const ShaderUniform Model;
    static const ShaderUniform View;
    static const ShaderUniform Projection;
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
            case Enum::Texture0: return "uTexture0";
            case Enum::Texture1: return "uTexture1";
            case Enum::Texture2: return "uTexture2";
        }
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

/// Shader Vertex Attribute Objects
constexpr ShaderVertexAttrib ShaderVertexAttrib::Position{ ShaderVertexAttrib::Enum::Position };
constexpr ShaderVertexAttrib ShaderVertexAttrib::TexCoord{ ShaderVertexAttrib::Enum::TexCoord };
constexpr ShaderVertexAttrib ShaderVertexAttrib::Color{ ShaderVertexAttrib::Enum::Color };
constexpr ShaderVertexAttrib ShaderVertexAttrib::Model0{ ShaderVertexAttrib::Enum::Model0 };
constexpr ShaderVertexAttrib ShaderVertexAttrib::Model1{ ShaderVertexAttrib::Enum::Model1 };
constexpr ShaderVertexAttrib ShaderVertexAttrib::Model2{ ShaderVertexAttrib::Enum::Model2 };
constexpr ShaderVertexAttrib ShaderVertexAttrib::Model3{ ShaderVertexAttrib::Enum::Model3 };

/// Shader Uniform Objects
constexpr ShaderUniform ShaderUniform::Color{ ShaderUniform::Enum::Color };
constexpr ShaderUniform ShaderUniform::Model{ ShaderUniform::Enum::Model };
constexpr ShaderUniform ShaderUniform::View{ ShaderUniform::Enum::View };
constexpr ShaderUniform ShaderUniform::Projection{ ShaderUniform::Enum::Projection };
constexpr ShaderUniform ShaderUniform::Texture0{ ShaderUniform::Enum::Texture0 };
constexpr ShaderUniform ShaderUniform::Texture1{ ShaderUniform::Enum::Texture1 };
constexpr ShaderUniform ShaderUniform::Texture2{ ShaderUniform::Enum::Texture2 };

// clang-format on

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_SHADER_VARIABLES_H_
