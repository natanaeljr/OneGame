#ifndef FIRSTGAME_UTIL_SIZE_H_
#define FIRSTGAME_UTIL_SIZE_H_

#include <tuple>
#include <type_traits>

namespace firstgame::util {

template<typename T>
struct WidthT {
    static_assert(std::is_arithmetic_v<T>);
    using type = T;

    explicit WidthT(T width) : width(width) {}
    template<typename U>
    explicit WidthT(U width) : width(static_cast<T>(width))
    {
    }
    operator T() const { return width; }

    WidthT(WidthT&&) noexcept = default;
    WidthT(const WidthT&) = default;
    WidthT& operator=(WidthT&&) noexcept = default;
    WidthT& operator=(const WidthT&) = default;

    template<typename U>
    explicit WidthT(const WidthT<U>& other) : width(static_cast<T>(other.operator U()))
    {
    }
    template<typename U>
    WidthT& operator=(const WidthT<U>& other)
    {
        width = static_cast<T>(other.operator U());
    }

   private:
    T width;
};

template<typename T>
WidthT(T) -> WidthT<T>;

using Width = WidthT<std::size_t>;

template<typename T>
struct HeightT {
    static_assert(std::is_arithmetic_v<T>);
    using type = T;

    explicit HeightT(T height) : height(height) {}
    template<typename U>
    explicit HeightT(U height) : height(static_cast<T>(height))
    {
    }
    operator T() const { return height; }

    HeightT(HeightT&&) noexcept = default;
    HeightT(const HeightT&) = default;
    HeightT& operator=(HeightT&&) noexcept = default;
    HeightT& operator=(const HeightT&) = default;

    template<typename U>
    explicit HeightT(const HeightT<U>& other) : height(static_cast<T>(other.operator U()))
    {
    }
    template<typename U>
    HeightT& operator=(const HeightT<U>& other)
    {
        height = static_cast<T>(other.operator U());
    }

   private:
    T height;
};

template<typename T>
HeightT(T) -> HeightT<T>;

using Height = HeightT<std::size_t>;

template<typename T>
struct SizeT {
    static_assert(std::is_arithmetic_v<T>);
    using type = T;

    WidthT<T> width;
    HeightT<T> height;

    SizeT(WidthT<T> width, HeightT<T> height) : width(width), height(height) {}

    template<typename U>
    SizeT(WidthT<U> width, HeightT<U> height)
        : width(static_cast<T>(width)), height(static_cast<T>(height))
    {
    }

    SizeT(SizeT&&) noexcept = default;
    SizeT(const SizeT&) = default;
    SizeT& operator=(SizeT&&) noexcept = default;
    SizeT& operator=(const SizeT&) = default;

    template<typename U>
    SizeT(const SizeT<U>& other) : width(WidthT<T>(other.width)), height(HeightT<T>(other.height))
    {
    }
    template<typename U>
    SizeT& operator=(const SizeT<U>& other)
    {
        width = WidthT<T>(other.width);
        height = HeightT<T>(other.height);
    }

    [[nodiscard]] constexpr auto tie() const { return std::tie(width, height); }
    [[nodiscard]] constexpr auto tuple() const { return std::make_tuple(width, height); }

    [[nodiscard]] constexpr T aspect_ratio() const { return width / height; }
};

template<typename T>
SizeT(T, T) -> SizeT<T>;

using Size = SizeT<std::size_t>;

}  // namespace firstgame::util

#endif  // FIRSTGAME_UTIL_SIZE_H_
