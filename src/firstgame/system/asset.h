#ifndef FIRSTGAME_SYSTEM_ASSET_H_
#define FIRSTGAME_SYSTEM_ASSET_H_

#include <memory>
#include "log.h"
#include "firstgame/platform/filesystem.h"

namespace firstgame::system {

/// Asset Interface
class Asset final {
   public:
    // Interface
    [[nodiscard]] auto ReadToString() -> std::string { return file_->ReadToString(); }

    // Con/Destructor
    explicit Asset(std::unique_ptr<platform::File>&& file, std::filesystem::path path)
        : path_(std::move(path)), file_(std::move(file))
    {
        TRACE("Asset file opened ({})", path_.c_str());
    }
    ~Asset() { TRACE("Asset file closed ({})", path_.c_str()); }

    // Copy/Move
    Asset(Asset&&) = default;
    Asset(const Asset&) = delete;
    Asset& operator=(Asset&&) = default;
    Asset& operator=(const Asset&) = delete;

   private:
    std::filesystem::path path_;
    std::unique_ptr<platform::File> file_;
};

}  // namespace firstgame::system

#endif  // FIRSTGAME_SYSTEM_ASSET_H_
