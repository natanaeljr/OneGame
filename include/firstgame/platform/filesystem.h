#ifndef FIRSTGAME_FILESYSTEM_H_
#define FIRSTGAME_FILESYSTEM_H_

#include <memory>
#include <string>
#include <filesystem>

namespace firstgame::platform {

/// File Interface
class File {
   public:
    // Interface
    virtual auto ReadToString() -> std::string = 0;
    virtual void Close() = 0;
    virtual auto Path() -> std::filesystem::path = 0;
    // Destructor
    virtual ~File() = default;

   protected:
    // Constructor
    File() = default;
};

/// File System Interface
class FileSystem {
   public:
    // Interface
    virtual auto Open(const char* filename) -> std::unique_ptr<File> = 0;
    // Destructor
    virtual ~FileSystem() = default;

   protected:
    // Constructor
    FileSystem() = default;
};

}  // namespace firstgame::platform

#endif  // FIRSTGAME_FILESYSTEM_H_
