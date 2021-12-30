#ifndef FIRSTGAME_OPENGL_SHADER_H_
#define FIRSTGAME_OPENGL_SHADER_H_

#include <string>
#include <optional>
#include <variant>
#include "firstgame/util/scoped.h"
#include "firstgame/opengl/gl/enum.h"
#include "firstgame/opengl/gl/types.h"
#include "firstgame/opengl/shader_vars.h"

namespace firstgame::opengl {

/// GLShader represents an OpenGL shader program with utility functions
/// to build the shader, bind/unbind, get uniform and attribute location and trace requests/execution.
/// Example:
/// ```
///  auto main_shader = GLShader::build("main", {vertex, fragment}).Assert();
///  main_shader->bind();
/// ```
/// The return type from build is Scoped because if build fails the returned object
/// is empty and that can be checked with exists() or asserted with .Assert().
/// Neither move or copy is allowed, so as to simplify shader ID ownership.
/// Nonetheless, Scoped allows for the GLShader object to be moved safely.
class GLShader final {
   public:
    explicit GLShader(std::string name);
    ~GLShader();
    GLShader(const GLShader&) = delete;
    GLShader& operator=(const GLShader&) = delete;

   public:
    /// Get shader program name
    [[nodiscard]] std::string_view name() const { return name_; }

    /// Bind shader program
    void bind();

    /// Unbind shader program
    void unbind();

    /// Get attribute location
    [[nodiscard]] GLint attr_loc(GLAttr attr) const;

    /// Get uniform location
    [[nodiscard]] GLint unif_loc(GLUnif unif) const;

    /// Load attributes' location into local array
    void load_attr_loc(const std::initializer_list<struct GLAttrInfo>& list);

    /// Load uniforms' location into local array
    void load_unif_loc(const std::initializer_list<struct GLUnifInfo>& list);

   public:
    /// Build the shader program from sources
    static auto build(std::string name, const struct ShaderSourceArray& sources) -> util::Scoped<GLShader>;

   private:
    /// Program name
    std::string name_;
    /// Program ID
    unsigned int id_;
    /// Attributes' location
    GLint attrs[static_cast<size_t>(GLAttr::COUNT)] = { -1 };
    /// Uniforms' location
    GLint unifs[static_cast<size_t>(GLUnif::COUNT)] = { -1 };
};

/// Input Attribute Info for GLShader::load_attr_loc().
struct GLAttrInfo {
    GLAttr attr;                                     ///< One of the attributes supported by the engine
    std::variant<GLint, std::string_view> loc_name;  ///< Either the hardcoded location or variable name
};

/// Input Uniform Info for GLShader::load_unif_loc().
struct GLUnifInfo {
    GLUnif unif;                                     ///< One of the uniforms supported by the engine
    std::variant<GLint, std::string_view> loc_name;  ///< Either the hardcoded location or variable name
};

/// Input shader sources array for GLShader::build.
struct ShaderSourceArray {
    std::string_view vertex;    // required
    std::string_view fragment;  // required
    std::optional<std::string_view> geometry = std::nullopt;
};

/// Stringify opengl shader type.
auto shader_type_str(GLenum shader_type) -> std::string_view;

}  // namespace firstgame::opengl
#endif  // FIRSTGAME_OPENGL_SHADER_H_
