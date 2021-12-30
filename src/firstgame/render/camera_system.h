////////////////////////////////////////////////////////////////////////////////////////////////////
/// This file defines a camera system for controlling all cameras needed in the engine.
/// It is therefore made available an interface for lettings events control the cameras.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FIRSTGAME_RENDER_CAMERA_SYSTEM_H_
#define FIRSTGAME_RENDER_CAMERA_SYSTEM_H_

#include "firstgame/util/size.h"
#include "firstgame/util/direction.h"
#include "camera_perspective.h"
#include "camera_orthographic.h"
#include "render_pass.h"
#include "firstgame/opengl/shader.h"

namespace firstgame::render {

/// Camera System encapsulates the various cameras inside a class.
/// The interface acts as a bridge between the event system handling and the camera objects.
/// Therefore, data common to all cameras are stored here, then Higher-level code manipulates
/// the camera array through this interface, independently of the camera types.
class CameraSystem {
   public:
    explicit CameraSystem(util::Size size) : perspective_(size), orthographic_(size) {}

    /// Interface
    void OnResize(util::Size size)
    {
        perspective_.Resize(size);
        orthographic_.Resize(size);
    }

    void OnZoom(float offset)
    {
        perspective_.Zoom(offset);
        orthographic_.Zoom(offset);
    }

    void OnMove(util::MoveDirection direction, float deltatime) { perspective_.Translate(direction, deltatime); }

    void OnPoint(float xpos, float ypos)
    {
        if (last_xpos == 0 && last_ypos == 0) {
            last_xpos = xpos;
            last_ypos = ypos;
        }
        float xoffset = xpos - last_xpos;
        float yoffset = last_ypos - ypos;  // reverse y since it goes from bottom to top in opengl
        perspective_.Rotate(xoffset, yoffset);
        last_xpos = xpos;
        last_ypos = ypos;
    }

    void Render(RenderPass pass, opengl::GLShader& shader) const;

   private:
    CameraPerspective perspective_;
    CameraOrthographic orthographic_;
    float last_xpos = 0;
    float last_ypos = 0;
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_SYSTEM_H_
