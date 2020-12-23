/**
 * Implementation file for the FirstGame Interface.
 */

#include "firstgame/firstgame.h"

#include "firstgame/event.h"
#include "firstgame/system/log.h"
#include "firstgame/system/system.h"
#include "firstgame/renderer/renderer.h"

namespace firstgame {

/**************************************************************************************************/

//! Class that implements the FirstGame interface
class FirstGameImpl : public FirstGame {
   public:
    explicit FirstGameImpl(std::shared_ptr<spdlog::logger> logger);
    void Update(unsigned int timestep) override;
    void OnImGuiRender() override;
    void OnEvent(const Event& event) override;
    ~FirstGameImpl() override;

   private:
    system::System system_;
    renderer::Renderer renderer_;
};

/**************************************************************************************************/

FirstGameImpl::FirstGameImpl(std::shared_ptr<spdlog::logger> logger) : system_(std::move(logger))
{
    TRACE("Created FirstGameImpl");
}

/**************************************************************************************************/

void FirstGameImpl::Update(unsigned int timestep)
{
    renderer_.Render();
}

/**************************************************************************************************/

void FirstGameImpl::OnImGuiRender()
{
    // ImGui::ShowDemoWindow();
}

/**************************************************************************************************/

void FirstGameImpl::OnEvent(const Event& event)
{
    TRACE("Event Received!");
    // TODO: Handle mouse/keyboard/touch and window events
}

/**************************************************************************************************/

FirstGameImpl::~FirstGameImpl()
{
    TRACE("Destroyed FirstGameImpl");
}

/**************************************************************************************************/

auto FirstGame::New(std::shared_ptr<spdlog::logger> logger) -> std::unique_ptr<FirstGame>
{
    return std::make_unique<FirstGameImpl>(std::move(logger));
}

}  // namespace firstgame