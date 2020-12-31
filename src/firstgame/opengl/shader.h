#ifndef FIRSTGAME_OPENGL_SHADER_H_
#define FIRSTGAME_OPENGL_SHADER_H_

#include <cstring>
#include <memory>
#include <gsl/gsl_util>
#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"
#include "firstgame/util/scoped.h"

namespace firstgame::opengl {

/**************************************************************************************************/

struct Shader final {
    GLuint program;
    static auto Build(const char* vertex_src, const char* fragment_src) -> util::Scoped<Shader>;

    Shader() : program(glCreateProgram()) { TRACE("Created Shader {}", program); }
    ~Shader()
    {
        glDeleteProgram(program);
        TRACE("Destroyed Shader {}", program);
    }

    Shader(Shader&&) = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;

    static const char* Str(GLenum shader_type);

   private:
    static auto Compile(GLenum shader_type, const char* shader_src)
        -> util::Scoped<GLuint, std::decay_t<decltype(glDeleteShader)>>;
    [[nodiscard]] bool Link(GLuint vertex_shader, GLuint fragment_shader) const;
};

/**************************************************************************************************/

auto Shader::Build(const char* vertex_src, const char* fragment_src) -> util::Scoped<Shader>
{
    auto vertex = Shader::Compile(GL_VERTEX_SHADER, vertex_src);
    auto fragment = Shader::Compile(GL_FRAGMENT_SHADER, fragment_src);
    if (not vertex || not fragment) {
        ERROR("Failed to Compile Shaders");
        return {};
    }
    auto shader = util::make_scoped<Shader>();
    if (not shader->Link(*vertex, *fragment)) {
        ERROR("Failed to Link Shader Program");
        return {};
    }
    return shader;
}

/**************************************************************************************************/

auto Shader::Compile(GLenum shader_type, const char* shader_src)
    -> util::Scoped<GLuint, std::decay_t<decltype(glDeleteShader)>>
{
    auto shader = util::make_scoped_final<GLuint>(&glDeleteShader, glCreateShader(shader_type));
    glShaderSource(*shader, 1, &shader_src, nullptr);
    glCompileShader(*shader);
    GLint info_len = 0;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len) {
        auto info = std::make_unique<char[]>(info_len);
        glGetShaderInfoLog(*shader, info_len, nullptr, info.get());
        DEBUG("{} Compilation Output:\n{}", Shader::Str(shader_type), info.get());
    }
    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        ERROR("Failed to Compile {}", Shader::Str(shader_type));
        return {};
    }
    return shader;
}

/**************************************************************************************************/

bool Shader::Link(GLuint vertex_shader, GLuint fragment_shader) const
{
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint info_len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len) {
        auto info = std::make_unique<char[]>(info_len);
        glGetProgramInfoLog(program, info_len, nullptr, info.get());
        DEBUG("Shader Program Link Output:\n{}", info.get());
    }
    GLint link_status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (!link_status) {
        ERROR("Failed to Link Shader Program");
    }
    glDetachShader(program, fragment_shader);
    glDetachShader(program, vertex_shader);
    return link_status;
}

/**************************************************************************************************/

const char* Shader::Str(GLenum shader_type)
{
    switch (shader_type) {
        case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
        default: ASSERT_MSG(0, "Invalid shader type {}", shader_type); return "<invalid>";
    }
}

}  // namespace firstgame::opengl

#endif  // FIRSTGAME_OPENGL_SHADER_H_
