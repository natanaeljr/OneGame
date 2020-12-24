#ifndef FIRSTGAME_SYSTEM_ASSET_MGR_H_
#define FIRSTGAME_SYSTEM_ASSET_MGR_H_

#include <memory>
#include <filesystem>

#include "log.h"
#include "asset.h"
#include "firstgame/util/scoped.h"
#include "firstgame/util/currenton.h"
#include "firstgame/platform/filesystem.h"

namespace firstgame::system {

/// Asset Manager Interface
class AssetManager final : public util::Currenton<AssetManager> {
   public:
    // Interface
    [[nodiscard]] auto Open(std::filesystem::path assetpath) -> util::Scoped<Asset>;

    // Con/Destructor
    explicit AssetManager(std::shared_ptr<platform::FileSystem> filesystem)
        : filesystem_(std::move(filesystem))
    {
        TRACE("Initialized AssetManager");
    }
    ~AssetManager() override { TRACE("De-initialized AssetManager"); }

    // Copy/Move
    AssetManager(AssetManager&&) = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(AssetManager&&) = default;
    AssetManager& operator=(const AssetManager&) = delete;

   private:
    std::shared_ptr<platform::FileSystem> filesystem_;
};

}  // namespace firstgame::system

#endif  // FIRSTGAME_SYSTEM_ASSET_MGR_H_
