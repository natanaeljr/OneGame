/**
 * Implementation file for the FirstGame Interface.
 */

#include "firstgame/firstgame.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>
#include <firstgame/render/motion.h>

#include "firstgame/event/event.h"
#include "firstgame/system/log.h"
#include "firstgame/system/system.h"
#include "firstgame/render/painter.h"
#include "firstgame/render/renderer.h"
#include "firstgame/render/renderable.h"
#include "firstgame/render/transform.h"
#include "firstgame/util/overloaded.h"

namespace firstgame {

/**************************************************************************************************/

//! Class that implements the FirstGame interface
class FirstGameImpl final : public FirstGame {
   public:
    explicit FirstGameImpl(int width, int height, std::shared_ptr<spdlog::logger> logger,
                           std::shared_ptr<platform::FileSystem> filesystem);
    void Update(float deltatime) override;
    void OnImGuiRender() override;
    void OnEvent(const event::Event& event) override;
    ~FirstGameImpl() override;

   private:
    system::System system_;
    render::Renderer renderer_;
    entt::registry registry_;
};

/**************************************************************************************************/

FirstGameImpl::FirstGameImpl(int width, int height, std::shared_ptr<spdlog::logger> logger,
                             std::shared_ptr<platform::FileSystem> filesystem)
    : system_(std::move(logger), std::move(filesystem)),
      renderer_({ util::Width(width), util::Height(height) })
{
    TRACE("Created FirstGameImpl");

    // Generate Multiple Quads
    // for (float i : { 1.f, 2.f, 3.f, 4.f, 5.f }) {
    //     float x = ((2.0f / 5) * i) - 1.0f - (2.0f / 5 / 2);
    //     entt::handle quad{ registry_, registry_.create() };
    //     quad.emplace<render::Renderable>(render::GenerateQuad());
    //     quad.emplace<render::Transform>(render::Transform{
    //         .position = glm::vec3(x, 0.0f, 0.0f),
    //         .scale = glm::vec3(0.12f),
    //     });
    // }

    // Generate many cubes
    // for (float x = 0; x < 100.0f; x++) {
    //     for (float z = 0; z < 100.0f; z++) {
    //         entt::handle cube{ registry_, registry_.create() };
    //         cube.emplace<render::Renderable>(render::GenerateCube());
    //         cube.emplace<render::Transform>(render::Transform{
    //             .position = glm::vec3(x, -3.0f, z),
    //             .scale = glm::vec3(0.5f),
    //             .rotation = glm::quat(1.0f, glm::vec3(0.0f)),
    //         });
    //     }
    // }

    // Generate instanced cubes
    entt::handle cubes{ registry_, registry_.create() };
    cubes.emplace<render::RenderableInstanced>(render::GenerateCubeInstanced(50, 100));

    // Generate Single Quad
    entt::handle quad{ registry_, registry_.create() };
    quad.emplace<render::Renderable>(render::GenerateQuad());
    quad.emplace<render::Transform>(render::Transform{
        .position = glm::vec3(-7.0f, 0.0f, 10.0f),
        .scale = glm::vec3(1.0f),
        .rotation = glm::quat(1.0f, glm::vec3(0.0f)),
    });
    quad.emplace<render::Motion>(render::Motion{
        .velocity = glm::vec3(0.0f, 0.0f, 40.0f),
        .acceleration = glm::vec3(0.0f, 0.0f, 15.0f),
    });

    // Generate Cube
    entt::handle cube{ registry_, registry_.create() };
    cube.emplace<render::Renderable>(render::GenerateCube());
    cube.emplace<render::Transform>(render::Transform{
        .position = glm::vec3(-7.0f, 0.0f, 0.0f),
        .scale = glm::vec3(1.0f),
        .rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    });
    cube.emplace<render::Motion>(render::Motion{
        .velocity = glm::vec3(70.0f, 50.0f, 90.0f),
        .acceleration = glm::vec3(0.0f),
    });
}

/**************************************************************************************************/

void FirstGameImpl::Update(float deltatime)
{
    auto view = registry_.view<render::Transform, render::Motion>();
    view.each([deltatime](render::Transform& transform, render::Motion& motion) {
        motion.velocity += motion.acceleration * deltatime;
        glm::vec3 degrees = motion.velocity * deltatime;
        transform.rotation *= glm::angleAxis(glm::radians(degrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform.rotation *= glm::angleAxis(glm::radians(degrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform.rotation *= glm::angleAxis(glm::radians(degrees.z), glm::vec3(0.0f, 0.0f, 1.0f));
    });

    renderer_.Render(registry_);
}

/**************************************************************************************************/

void FirstGameImpl::OnImGuiRender()
{
    ImGui::Begin("Stats");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
}

/**************************************************************************************************/

void FirstGameImpl::OnEvent(const event::Event& event)
{
    std::visit(
        util::Overloaded{
            [this](const event::KeyEvent& key) {
                TRACE("Event Received: Key {{key: {}, action: {}}}", key.key, key.action);
                renderer_.OnKeystroke(key, 0.016f);
            },
            [this](const event::CursorEvent& cursor) {
                TRACE("Event Received: Cursor {{xpos: {}, ypos: {}}}", cursor.xpos, cursor.ypos);
                renderer_.OnCursorMove(cursor.xpos, cursor.ypos);
            },
            [](const event::MouseEvent& mouse) {
                TRACE("Event Received: Mouse {{button: {}, action: {}}}", mouse.button,
                      mouse.action);
            },
            [this](const event::ScrollEvent& scroll) {
                TRACE("Event Received: Scroll {{xoffset: {}, yoffset: {}}}", scroll.xoffset,
                      scroll.yoffset);
                renderer_.OnScroll(scroll.yoffset);
            },
            [](const event::JoystickEvent& joystick_event) { TRACE("Event Received: Joystick"); },
            [this](const event::WindowEvent& window_event) {
                std::visit(util::Overloaded{
                               [](const event::WindowEvent::Focus& focus) {
                                   TRACE("Event Received: Window::Focus");
                               },
                               [](const event::WindowEvent::Imize& imize) {
                                   TRACE("Event Received: Window::Imize");
                               },
                               [this](const event::WindowEvent::Resize& resize) {
                                   TRACE("Event Received: Window::Resize");
                                   renderer_.OnResize(
                                       { util::Width(resize.width), util::Height(resize.height) });
                               },
                           },
                           window_event.variant);
            },
        },
        event);
}

/**************************************************************************************************/

FirstGameImpl::~FirstGameImpl()
{
    TRACE("Destroyed FirstGameImpl");
}

/**************************************************************************************************/

auto FirstGame::New(int width, int height, std::shared_ptr<spdlog::logger> logger,
                    std::shared_ptr<platform::FileSystem> filesystem) -> std::unique_ptr<FirstGame>
{
    return std::make_unique<FirstGameImpl>(width, height, std::move(logger), std::move(filesystem));
}

}  // namespace firstgame