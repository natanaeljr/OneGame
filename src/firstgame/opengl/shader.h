#ifndef FIRSTGAME_OPENGL_SHADER_H_
#define FIRSTGAME_OPENGL_SHADER_H_

#include <cstring>
#include <new>
#include <string_view>
#include "firstgame/opengl/gl.h"
#include "firstgame/system/log.h"
#include "firstgame/util/scoped.h"

namespace firstgame::opengl {

/**************************************************************************************************/

struct Shader {
   public:
    GLuint program;
    static auto Build(std::string_view vextex_src, std::string_view fragment_src)
        -> util::Scoped<Shader>;

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
};

/**************************************************************************************************/

auto Shader::Build(std::string_view vextex_src, std::string_view fragment_src)
    -> util::Scoped<Shader>
{
    glCreateShader(GL_VERTEX_SHADER);

    glCreateShader(GL_FRAGMENT_SHADER);

    auto shader = util::make_scoped<Shader>();
    return shader;
}

}  // namespace firstgame::opengl

#endif  // FIRSTGAME_OPENGL_SHADER_H_
