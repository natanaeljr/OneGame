#include "firstgame/firstgame.h"

#include <imgui/imgui.h>

#include "firstgame/event.h"
#include "internal/log.h"

namespace firstgame {

// Game Implementation Class
class FirstGameImpl : public FirstGame {
   public:
    explicit FirstGameImpl(std::shared_ptr<spdlog::logger> logger);
    void Update(unsigned int timestep) override;
    void OnImGuiRender() override;
    void OnEvent(const Event& event) override;
    ~FirstGameImpl() override;
};

FirstGameImpl::FirstGameImpl(std::shared_ptr<spdlog::logger> logger)
{
    assert(::firstgame::g_logger == nullptr);
    ::firstgame::g_logger = std::move(logger);

    TRACE("Created FirstGameImpl");
}

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
    TRACE("Event Received!");
    // TODO: Handle mouse/keyboard/touch and window events
}

FirstGameImpl::~FirstGameImpl()
{
    ::firstgame::g_logger.reset();
}

auto FirstGame::New(std::shared_ptr<spdlog::logger> logger) -> std::unique_ptr<FirstGame>
{
    return std::make_unique<FirstGameImpl>(std::move(logger));
}

}  // namespace firstgame