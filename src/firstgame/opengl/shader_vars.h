#ifndef FIRSTGAME_OPENGL_VARS_H_
#define FIRSTGAME_OPENGL_VARS_H_

namespace firstgame::opengl {

/// Enumeration of supported GL Shader Attributes
enum class GLAttr {
    POSITION = 0,
    TEXCOORD,
    COLOR,
    MODEL,
    // must be last
    COUNT,
};

/// Enumeration of supported GL Shader Uniforms
enum class GLUnif {
    COLOR = 0,
    MODEL,
    VIEW,
    PROJECTION,
    TEXTURE0,
    TEXTURE1,
    TEXTURE2,
    TEXTURE3,
    // must be last
    COUNT,
};

}  // namespace firstgame::opengl
#endif  // FIRSTGAME_OPENGL_VARS_H_
