#ifndef FIRSTGAME_OPENGL_SHADER_H_
#define FIRSTGAME_OPENGL_SHADER_H_

#include <string>
#include <optional>
#include "firstgame/util/scoped.h"
#include "firstgame/opengl/gl/enum.h"
#include "firstgame/opengl/gl/types.h"

namespace firstgame::opengl {

/// Shader represents an OpenGL shader program with utility functions
/// to build the shader, bind/unbind and trace requests/execution.
/// Example:
/// ```
///  auto main_shader = Shader::Build("main", {vertex, fragment}).Assert();
///  main_shader->Bind();
/// ```
/// The return type from Build is Scoped because if build fails the returned object
/// is empty and that can be checked with exists() or asserted with .Assert().
/// Neither move or copy is allowed, so as to simplify shader ID ownership.
/// Nonetheless, Scoped allows for the Shader object to be moved safely.
class Shader final {
   public:
    explicit Shader(std::string name);
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

   public:
    /// Get shader program name
    [[nodiscard]] std::string_view Name() const { return name; }

    /// Bind shader program
    void Bind();

    /// Unbind shader program
    void Unbind();

   public:
    /// Build the shader program from sources
    static auto Build(std::string name, const struct ShaderSourceArray& sources) -> util::Scoped<Shader>;

   private:
    /// Program ID
    unsigned int id;
    /// Program Name
    std::string name;
};

/// Input shader sources array for Shader::Build.
struct ShaderSourceArray {
    std::string_view vertex;    // required
    std::string_view fragment;  // required
    std::optional<std::string_view> geometry = std::nullopt;
};

/// Stringify opengl shader type.
auto ShaderTypeStr(GLenum shader_type) -> std::string_view;

}  // namespace firstgame::opengl
#endif  // FIRSTGAME_OPENGL_SHADER_H_
