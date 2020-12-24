/**
 * System interface for the rest of FirstGame.
 */

#ifndef FIRSTGAME_SYSTEM_SYSTEM_H_
#define FIRSTGAME_SYSTEM_SYSTEM_H_

#include "log.h"
#include "asset.h"
#include "firstgame/util/currenton.h"
#include "firstgame/platform/filesystem.h"

namespace firstgame::system {

class System final : public util::Currenton<System> {
   public:
    // Interface
    auto Logger() -> Logger& { return logger_; }
    auto AssetManager() -> AssetManager& { return asset_mgr_; }
    auto FileSystem() -> platform::FileSystem& { return *filesystem_; }

    // Constructor
    explicit System(std::shared_ptr<spdlog::logger> logger,
                    std::shared_ptr<platform::FileSystem> filesystem)
        : logger_(std::move(logger)), asset_mgr_(filesystem), filesystem_(std::move(filesystem))
    {
        TRACE("Initialized System");
    }
    // Destructor
    ~System() override { TRACE("De-initialized System"); }

   private:
    system::Logger logger_;
    system::AssetManager asset_mgr_;
    std::shared_ptr<platform::FileSystem> filesystem_;
};

}  // namespace firstgame::system

#endif  // FIRSTGAME_SYSTEM_SYSTEM_H_
