#ifndef FIRSTGAME_OPENGL_GL_TYPES_H_
#define FIRSTGAME_OPENGL_GL_TYPES_H_

#if defined(FIRSTGAME_OPENGL_ES3)
#include <GLES3/gl3.h>
#elif defined(FIRSTGAME_OPENGL_GLAD)
#include <glad/glad.h>
#elif defined(FIRSTGAME_OPENGL_GLBINDING3)
#include <glbinding/gl/types.h>
using namespace gl;
#else
#error "No OpenGL library specified!"
#endif

#endif  // FIRSTGAME_OPENGL_GL_TYPES_H_
