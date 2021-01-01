#ifndef FIRSTGAME_FIRSTGAME_H_
#define FIRSTGAME_FIRSTGAME_H_

#include <memory>
#include <spdlog/logger.h>
#include "firstgame/event/event.h"
#include "firstgame/platform/filesystem.h"

namespace firstgame {

/// Game Interface
class FirstGame {
   public:
    static auto New(int width, int height, std::shared_ptr<spdlog::logger> logger,
                    std::shared_ptr<platform::FileSystem> filesystem) -> std::unique_ptr<FirstGame>;
    // Interface
    virtual void Update(unsigned int timestep) = 0;
    virtual void OnImGuiRender() = 0;
    virtual void OnEvent(const event::Event& event) = 0;
    // Destructor
    virtual ~FirstGame() = default;

   protected:
    FirstGame() = default;
};

}  // namespace firstgame

#endif  // FIRSTGAME_FIRSTGAME_H_
