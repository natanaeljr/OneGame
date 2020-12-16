#ifndef FIRSTGAME_OPENGL_GL_H_
#define FIRSTGAME_OPENGL_GL_H_

#if defined(FIRSTGAME_OPENGL_ES3)
#include <GLES3/gl3.h>
#elif defined(FIRSTGAME_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#else
#error "No OpenGL library specified!"
#endif

#endif  // FIRSTGAME_OPENGL_GL_H_
