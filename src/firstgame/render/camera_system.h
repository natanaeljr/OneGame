#ifndef FIRSTGAME_RENDER_CAMERA_SYSTEM_H_
#define FIRSTGAME_RENDER_CAMERA_SYSTEM_H_

#include "firstgame/util/size.h"
#include "camera_perspective.h"
#include "camera_orthographic.h"
#include "render_pass.h"

namespace firstgame::render {

struct CameraSystem {
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
    void Render(RenderPass pass) const;

   private:
    CameraPerspective perspective_;
    CameraOrthographic orthographic_;
};

}  // namespace firstgame::render

#endif  // FIRSTGAME_RENDER_CAMERA_SYSTEM_H_
