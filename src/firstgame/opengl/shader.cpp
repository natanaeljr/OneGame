#include "firstgame/opengl/shader.h"

#include <string>
#include <cstring>
#include <memory>

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

namespace firstgame::opengl {

// TODO:
//  - trace to specific shader category
//  - fix Shader::name string optimization moved.
//    Probably going to have to make Shader movable, and fix Scoped move logic

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward-declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

/// RAII for a shader id object created with glCreateShader
using ScopedShaderObj = util::Scoped<GLuint, std::decay_t<decltype(glDeleteShader)>>;

/// Compile shader source into an object
static auto CompileShader(GLenum shader_type, const char* shader_src) -> ScopedShaderObj;

/// Link shader objects into a program
[[nodiscard]] static bool LinkShader(GLuint program, const struct ShaderObjArray& shaders);

/// Input shader obj array for LinkShader
struct ShaderObjArray {
    GLuint vertex;    // required
    GLuint fragment;  // required
    std::optional<GLuint> geometry = std::nullopt;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader
////////////////////////////////////////////////////////////////////////////////////////////////////

Shader::Shader(std::string name) : id(glCreateProgram()), name(std::move(name))
{
    TRACE("New Shader program '{}' [{}]", this->name, id);
}

Shader::~Shader()
{
    glDeleteProgram(id);
    TRACE("Delete Shader program '{}' [{}]", name, id);
}

void Shader::Bind()
{
    // TRACE("Binding Shader program '{}' [{}]", name, id);
    glUseProgram(id);
}

void Shader::Unbind()
{
    // TRACE("Unbinding Shader program '{}' [{}]", name, id);
    glUseProgram(0);
}

auto Shader::Build(std::string name, const ShaderSourceArray& sources) -> util::Scoped<Shader>
{
    auto vertex = CompileShader(GL_VERTEX_SHADER, sources.vertex.data());
    auto fragment = CompileShader(GL_FRAGMENT_SHADER, sources.fragment.data());
    auto geometry = sources.geometry ? CompileShader(GL_GEOMETRY_SHADER, sources.geometry->data()) : ScopedShaderObj{};

    if (not vertex || not fragment || (sources.geometry && not geometry)) {
        ERROR("Failed to Compile Shaders");
        return {};
    }

    auto shader = util::make_scoped<Shader>(std::move(name));
    if (not LinkShader(shader->id, { *vertex, *fragment, (geometry ? std::optional(*geometry) : std::nullopt) })) {
        ERROR("Failed to Link Shader program");
        return {};
    }

    DEBUG("Compiled & Linked Shader program '{}' [{}]", shader->name, shader->id);
    return shader;
}

auto CompileShader(GLenum shader_type, const char* shader_src) -> ScopedShaderObj
{
    auto shader = util::make_scoped_final<GLuint>(&glDeleteShader, glCreateShader(shader_type));
    glShaderSource(*shader, 1, &shader_src, nullptr);
    glCompileShader(*shader);

    GLint info_len = 0;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len) {
        auto info = std::make_unique<char[]>(info_len);
        glGetShaderInfoLog(*shader, info_len, nullptr, info.get());
        DEBUG("{} Compilation Output:\n{}", ShaderTypeStr(shader_type), info.get());
    }

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        ERROR("Failed to Compile {}", ShaderTypeStr(shader_type));
        return {};
    }

    return shader;
}

bool LinkShader(GLuint program, const struct ShaderObjArray& shaders)
{
    glAttachShader(program, shaders.vertex);
    glAttachShader(program, shaders.fragment);
    if (shaders.geometry)
        glAttachShader(program, *shaders.geometry);
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

    glDetachShader(program, shaders.vertex);
    glDetachShader(program, shaders.fragment);
    if (shaders.geometry)
        glDetachShader(program, *shaders.geometry);

    return link_status;
}

auto ShaderTypeStr(GLenum shader_type) -> std::string_view
{
    switch (shader_type) {
        case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
        case GL_TESS_EVALUATION_SHADER: return "GL_TESS_EVALUATION_SHADER";
        case GL_TESS_CONTROL_SHADER: return "GL_TESS_CONTROL_SHADER";
        case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
        default: ASSERT_MSG(0, "Invalid shader type {}", shader_type); return "<invalid>";
    }
}

}  // namespace firstgame::opengl
