/**
 * System interface for the rest of FirstGame.
 */

#ifndef FIRSTGAME_SYSTEM_SYSTEM_H_
#define FIRSTGAME_SYSTEM_SYSTEM_H_

#include "log.h"
#include "firstgame/util/currenton.h"

namespace firstgame::system {

class System final : public util::Currenton<System> {
   public:
    explicit System(std::shared_ptr<spdlog::logger> logger) : logger_(std::move(logger))
    {
        TRACE("Initialized System");
    }
    ~System() override { TRACE("De-initialized System"); }

   private:
    Logger logger_;
};

}  // namespace firstgame::system

#endif  // FIRSTGAME_SYSTEM_SYSTEM_H_
