#ifndef FIRSTGAME_FIRSTGAME_H_
#define FIRSTGAME_FIRSTGAME_H_

#include <memory>
#include <spdlog/logger.h>

namespace firstgame {

// Game Interface
class FirstGame {
   public:
    static auto New(std::shared_ptr<spdlog::logger> logger) -> std::unique_ptr<FirstGame>;
    // Interface
    virtual void Update(unsigned int timestep) = 0;
    virtual void OnImGuiRender() = 0;
    virtual void OnEvent(const class Event& event) = 0;
    // Destructor
    virtual ~FirstGame() = default;
};

}  // namespace firstgame

#endif  // FIRSTGAME_FIRSTGAME_H_
