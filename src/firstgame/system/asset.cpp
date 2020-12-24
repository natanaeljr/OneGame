#include "asset.h"

#include "log.h"
#include "system.h"

#ifndef FIRSTGAME_ASSETS_DIR_PATH
#error "Not defined FirstGame Assets directory path!"
#endif

namespace firstgame::system {

/**************************************************************************************************/

Asset::Asset(std::unique_ptr<platform::File>&& file, std::filesystem::path path)
    : path_(std::move(path)), file_(std::move(file))
{
    TRACE("Asset file opened (\"{}\")", path_.c_str());
}

/**************************************************************************************************/

Asset::~Asset()
{
    TRACE("Asset file closed (\"{}\")", path_.c_str());
}

/**************************************************************************************************/

auto Asset::ReadToString() -> std::string
{
    return file_->ReadToString();
}

/**************************************************************************************************/

auto AssetManager::Open(std::filesystem::path assetpath) -> std::optional<Asset>
{
    static std::filesystem::path basepath(FIRSTGAME_ASSETS_DIR_PATH);
    std::filesystem::path fullpath = basepath / assetpath;
    std::unique_ptr<platform::File> file = filesystem_->Open(fullpath.c_str());
    if (not file) {
        ERROR("Failed to open asset file (\"{}\")", assetpath.c_str());
        return std::nullopt;
    }
    return std::make_optional<Asset>(std::move(file), std::move(assetpath));
}

}  // namespace firstgame::system