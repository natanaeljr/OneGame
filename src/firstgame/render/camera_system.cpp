////////////////////////////////////////////////////////////////////////////////////////////////////
/// Camera System's definitions.
/// For documentation, see the header file.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "camera_system.h"

#include <glm/gtc/type_ptr.hpp>
#include "firstgame/opengl/gl/boolean.h"
#include "firstgame/opengl/gl/functions.h"
#include "firstgame/opengl/shader.h"

namespace firstgame::render {

void CameraSystem::Render(RenderPass pass, opengl::GLShader& shader) const
{
    const ViewProjection& matrix = [this, pass] {
        switch (pass) {
            case RenderPass::_2D: return orthographic_.Matrix();
            case RenderPass::_3D: return perspective_.Matrix();
        }
        abort();  //< unreachable
    }();

    glUniformMatrix4fv(shader.unif_loc(opengl::GLUnif::VIEW), 1, GL_FALSE, glm::value_ptr(matrix.view));
    glUniformMatrix4fv(shader.unif_loc(opengl::GLUnif::PROJECTION), 1, GL_FALSE, glm::value_ptr(matrix.projection));
}

}  // namespace firstgame::render