/**
 * Implementation file for the FirstGame Interface.
 */

#include "firstgame/firstgame.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>

#include "firstgame/event/event.h"
#include "firstgame/system/log.h"
#include "firstgame/system/system.h"
#include "firstgame/renderer/helper.h"
#include "firstgame/renderer/renderer.h"
#include "firstgame/renderer/component.h"
#include "firstgame/renderer/transform.h"
#include "firstgame/util/overloaded.h"

namespace firstgame {

/**************************************************************************************************/

//! Class that implements the FirstGame interface
class FirstGameImpl final : public FirstGame {
   public:
    explicit FirstGameImpl(int width, int height, std::shared_ptr<spdlog::logger> logger,
                           std::shared_ptr<platform::FileSystem> filesystem);
    void Update(unsigned int timestep) override;
    void OnImGuiRender() override;
    void OnEvent(const event::Event& event) override;
    ~FirstGameImpl() override;

   private:
    system::System system_;
    renderer::Renderer renderer_;
    entt::registry registry_;
};

/**************************************************************************************************/

FirstGameImpl::FirstGameImpl(int width, int height, std::shared_ptr<spdlog::logger> logger,
                             std::shared_ptr<platform::FileSystem> filesystem)
    : system_(std::move(logger), std::move(filesystem)), renderer_(width, height)
{
    TRACE("Created FirstGameImpl");

    // Generate Quads
    for (float i : { 1.f, 2.f, 3.f, 4.f, 5.f }) {
        float x = ((2.0f / 5) * i) - 1.0f - (2.0f / 5 / 2);
        entt::handle quad{ registry_, registry_.create() };
        quad.emplace<renderer::RenderComponent>(renderer::GenerateQuad());
        quad.emplace<renderer::TransformComponent>(renderer::TransformComponent{
            .position = glm::vec3(x, 0.0f, 0.0f),
            .scale = glm::vec3(0.12f),
        });
    }
}

/**************************************************************************************************/

void FirstGameImpl::Update(unsigned int timestep)
{
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
            [](const event::KeyEvent& key_event) { TRACE("Event Received: Key"); },
            [](const event::CursorEvent& cursor_event) { TRACE("Event Received: Cursor"); },
            [](const event::MouseEvent& mouse_event) { TRACE("Event Received: Mouse"); },
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
                                   renderer_.ResizeCanvas(resize.width, resize.height);
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