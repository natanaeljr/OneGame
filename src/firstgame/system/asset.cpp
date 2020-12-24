#include "asset.h"
#include "system.h"

namespace firstgame::system {

/**************************************************************************************************/

auto Asset::ReadToString() -> std::string
{
    // TODO: Trace it.
    return file_->ReadToString();
}

/**************************************************************************************************/

auto AssetManager::Open(const char* filename, int mode) -> std::optional<Asset>
{
    // TODO: Trace it.
    std::unique_ptr<platform::File> file = filesystem_->Open(filename, mode);
    if (not file) {
        return std::nullopt;
    }
    return std::make_optional<Asset>(std::move(file));
}

}  // namespace firstgame::system