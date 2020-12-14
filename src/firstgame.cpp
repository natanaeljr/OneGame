#include "firstgame/firstgame.h"
#include "firstgame/event.h"
#include <imgui/imgui.h>

namespace firstgame {

// Game Implementation Class
class FirstGameImpl : public FirstGame {
   public:
    void Update(unsigned int timestep) override;
    void OnImGuiRender() override;
    void OnEvent(const Event& event) override;
    ~FirstGameImpl() override = default;
};

void FirstGameImpl::Update(unsigned int timestep)
{
    // TODO: Update game state
    // TODO: Render game state
}

void FirstGameImpl::OnImGuiRender()
{
    ImGui::ShowDemoWindow();
}

void FirstGameImpl::OnEvent(const Event& event)
{
    // TODO: Handle mouse/keyboard/touch and window events
}

auto FirstGame::New() -> std::unique_ptr<FirstGame>
{
    return std::make_unique<FirstGameImpl>();
}

}  // namespace firstgame