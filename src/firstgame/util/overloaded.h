#ifndef FIRSTGAME_UTIL_OVERLOADED_H_
#define FIRSTGAME_UTIL_OVERLOADED_H_

namespace firstgame::util {
// clang-format off

// helper type for the visitor
template<class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

// clang-format on
}  // namespace firstgame::util

#endif  // FIRSTGAME_UTIL_OVERLOADED_H_
