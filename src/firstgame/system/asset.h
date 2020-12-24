#ifndef FIRSTGAME_SYSTEM_ASSET_H_
#define FIRSTGAME_SYSTEM_ASSET_H_

#include <memory>
#include <string>
#include <optional>

#include "log.h"
#include "firstgame/util/currenton.h"
#include "firstgame/platform/filesystem.h"

namespace firstgame::system {

/// Asset Interface
class Asset final {
   public:
    // Interface
    [[nodiscard]] auto ReadToString() -> std::string;

    // Con/Destructor
    explicit Asset(std::unique_ptr<platform::File>&& file, std::filesystem::path path);
    ~Asset();

    // Copy/Move
    Asset(Asset&&) = default;
    Asset(const Asset&) = delete;
    Asset& operator=(Asset&&) = default;
    Asset& operator=(const Asset&) = delete;

   private:
    std::filesystem::path path_;
    std::unique_ptr<platform::File> file_;
};

/// Asset Manager Interface
class AssetManager final : public util::Currenton<AssetManager> {
   public:
    // Interface
    [[nodiscard]] auto Open(std::filesystem::path assetpath) -> std::optional<Asset>;

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

#endif  // FIRSTGAME_SYSTEM_ASSET_H_
