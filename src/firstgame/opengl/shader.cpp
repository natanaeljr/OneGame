#include "firstgame/opengl/shader.h"

#include <string>
#include <cstring>
#include <memory>

#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"

namespace firstgame::opengl {

// TODO:
//  - trace to specific shader category
//  - fix GLShader::name_ string optimization moved.
//    Probably going to have to make GLShader movable, and fix Scoped move logic

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward-declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

/// RAII for a shader id_ object created with glCreateShader
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
// GLShader
////////////////////////////////////////////////////////////////////////////////////////////////////

GLShader::GLShader(std::string name) : name_(std::move(name)), id_(glCreateProgram())
{
    TRACE("New GLShader program '{}' [{}]", name_, id_);
    // Workaround for std::string SSO when Scoped (move does not work)
    name_.reserve(sizeof(std::string) + 1);
}

GLShader::~GLShader()
{
    glDeleteProgram(id_);
    TRACE("Delete GLShader program '{}' [{}]", name_, id_);
}

void GLShader::bind()
{
    // TRACE("Binding GLShader program '{}' [{}]", name_, id_);
    glUseProgram(id_);
}

void GLShader::unbind()
{
    // TRACE("Unbinding GLShader program '{}' [{}]", name_, id_);
    glUseProgram(0);
}

GLint GLShader::attr_loc(GLAttr attr) const
{
    return attrs[static_cast<size_t>(attr)];
}

GLint GLShader::unif_loc(GLUnif unif) const
{
    return unifs[static_cast<size_t>(unif)];
}

void GLShader::load_attr_loc(const std::initializer_list<GLAttrInfo>& list)
{
    this->bind();
    for (const auto& item : list) {
        if (const GLint* loc = std::get_if<GLint>(&item.loc_name))
            attrs[static_cast<size_t>(item.attr)] = *loc;
        else {
            auto attr_name = std::get<std::string_view>(item.loc_name);
            const GLint new_loc = glGetAttribLocation(id_, attr_name.data());
            if (new_loc == -1) {
                CRITICAL("Failed to get location for attribute '{}' from GLShader '{}' [{}]", attr_name, name_, id_);
                std::abort();
            }
            else {
                TRACE("Read attribute '{}' location {}, from GLShader '{}' [{}]", attr_name, new_loc, name_, id_);
            }
            attrs[static_cast<size_t>(item.attr)] = new_loc;
        }
    }
}

void GLShader::load_unif_loc(const std::initializer_list<GLUnifInfo>& list)
{
    this->bind();
    for (const auto& item : list) {
        if (const GLint* loc = std::get_if<GLint>(&item.loc_name))
            unifs[static_cast<size_t>(item.unif)] = *loc;
        else {
            auto unif_name = std::get<std::string_view>(item.loc_name);
            const GLint new_loc = glGetUniformLocation(id_, unif_name.data());
            if (new_loc == -1) {
                CRITICAL("Failed to get location for uniform '{}' from GLShader '{}' [{}]", unif_name, name_, id_);
                std::abort();
            }
            else {
                TRACE("Read uniform '{}' location {}, from GLShader '{}' [{}]", unif_name, new_loc, name_, id_);
            }
            unifs[static_cast<size_t>(item.unif)] = new_loc;
        }
    }
}

auto GLShader::build(std::string name, const struct ShaderSourceArray& sources) -> util::Scoped<GLShader>
{
    auto vertex = CompileShader(GL_VERTEX_SHADER, sources.vertex.data());
    auto fragment = CompileShader(GL_FRAGMENT_SHADER, sources.fragment.data());
    auto geometry = sources.geometry ? CompileShader(GL_GEOMETRY_SHADER, sources.geometry->data()) : ScopedShaderObj{};

    if (not vertex || not fragment || (sources.geometry && not geometry)) {
        ERROR("Failed to Compile Shaders");
        return {};
    }

    auto shader = util::make_scoped<GLShader>(std::move(name));
    if (not LinkShader(shader->id_, { *vertex, *fragment, (geometry ? std::optional(*geometry) : std::nullopt) })) {
        ERROR("Failed to Link GLShader program");
        return {};
    }

    DEBUG("Compiled & Linked GLShader program '{}' [{}]", shader->name_, shader->id_);
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
        DEBUG("{} Compilation Output:\n{}", shader_type_str(shader_type), info.get());
    }

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        ERROR("Failed to Compile {}", shader_type_str(shader_type));
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
        DEBUG("GLShader Program Link Output:\n{}", info.get());
    }

    GLint link_status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (!link_status) {
        ERROR("Failed to Link GLShader Program");
    }

    glDetachShader(program, shaders.vertex);
    glDetachShader(program, shaders.fragment);
    if (shaders.geometry)
        glDetachShader(program, *shaders.geometry);

    return link_status;
}

auto shader_type_str(GLenum shader_type) -> std::string_view
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
