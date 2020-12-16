#ifndef FIRSTGAME_OPENGL_SHADER_H_
#define FIRSTGAME_OPENGL_SHADER_H_

#include <cstring>
#include <new>
#include <memory>
#include <string_view>
#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"
#include "firstgame/util/scoped.h"

namespace firstgame::opengl {

/**************************************************************************************************/

struct Shader {
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

   private:
    static GLuint Compile(GLenum shader_type, const char* shader_src);
    bool Link(GLuint vertex_shader, GLuint fragment_shader);
};

/**************************************************************************************************/

auto Shader::Build(const char* vertex_src, const char* fragment_src) -> util::Scoped<Shader>
{
    auto shader = util::make_scoped<Shader>();
    if (not shader->Link(Compile(GL_VERTEX_SHADER, vertex_src),
                         Compile(GL_FRAGMENT_SHADER, fragment_src))) {
        ERROR("Failed to build Shader");
        return {};
    }
    return shader;
}

/**************************************************************************************************/

GLuint Shader::Compile(GLenum shader_type, const char* shader_src)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, nullptr);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint info_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        if (info_len) {
            auto info = std::unique_ptr<char[]>(new char[info_len]);
            glGetShaderInfoLog(shader, info_len, nullptr, info.get());
            ERROR("Failed to Compile Shader [{}]: {}", shader_type, info.get());
        }
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

/**************************************************************************************************/

bool Shader::Link(GLuint vertex_shader, GLuint fragment_shader)
{
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (!link_status) {
        GLint info_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
        if (info_len) {
            auto info = std::unique_ptr<char[]>(new char[info_len]);
            glGetProgramInfoLog(program, info_len, nullptr, info.get());
            ERROR("Failed to Link Program: {}", info.get());
        }
        return false;
    }
    return true;
}

}  // namespace firstgame::opengl

#endif  // FIRSTGAME_OPENGL_SHADER_H_
