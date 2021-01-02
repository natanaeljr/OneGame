#include "camera_system.h"

#include <glm/gtc/type_ptr.hpp>
#include "firstgame/opengl/gl/boolean.h"
#include "firstgame/opengl/gl/functions.h"

#include "shader_variables.h"

namespace firstgame::render {

void CameraSystem::Render(RenderPass pass) const
{
    const ViewProjection& matrix = [this, pass] {
        switch (pass) {
            case RenderPass::_2D: return orthographic_.Matrix();
            case RenderPass::_3D: return perspective_.Matrix();
        }
        abort();
    }();

    glUniformMatrix4fv(ShaderUniform::View.location(), 1, GL_FALSE, glm::value_ptr(matrix.view));
    glUniformMatrix4fv(ShaderUniform::Projection.location(), 1, GL_FALSE,
                       glm::value_ptr(matrix.projection));
}

}  // namespace firstgame::render