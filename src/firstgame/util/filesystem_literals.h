#ifndef FIRSTGAME_UTIL_FILESYSTEM_LITERALS_H_
#define FIRSTGAME_UTIL_FILESYSTEM_LITERALS_H_

#include <filesystem>

namespace firstgame::util::filesystem_literals {

inline static std::filesystem::path operator"" _path(const char* str, size_t len)
{
    return { str, str + len };
}

}  // namespace firstgame::util::filesystem_literals

#endif  // FIRSTGAME_UTIL_FILESYSTEM_LITERALS_H_
