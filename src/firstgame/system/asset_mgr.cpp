#include "asset_mgr.h"

#include "log.h"
#include "system.h"

#ifndef FIRSTGAME_ASSETS_DIR_PATH
#error "Not defined FirstGame Assets directory path!"
#endif

namespace firstgame::system {

auto AssetManager::Open(std::filesystem::path assetpath) -> util::Scoped<Asset>
{
    static std::filesystem::path basepath(FIRSTGAME_ASSETS_DIR_PATH);
    std::filesystem::path fullpath = basepath / assetpath;
    std::unique_ptr<platform::File> file = filesystem_->Open(fullpath.c_str());
    if (not file) {
        ERROR("Failed to open asset file ({})", assetpath.c_str());
        return {};
    }
    return util::make_scoped<Asset>(std::move(file), std::move(assetpath));
}

}  // namespace firstgame::system