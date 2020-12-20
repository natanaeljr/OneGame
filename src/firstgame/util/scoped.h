/**
 * Scope utility.
 */

#ifndef FIRSTGAME_UTIL_SCOPED_H_
#define FIRSTGAME_UTIL_SCOPED_H_

namespace firstgame::util {

template<typename T>
class Scoped final {
   public:
    Scoped() = default;

    [[nodiscard]] T* get() noexcept { return reinterpret_cast<T*>(object); }
    [[nodiscard]] const T* get() const noexcept { return reinterpret_cast<T*>(object); }
    [[nodiscard]] T* operator->() noexcept { return reinterpret_cast<T*>(object); }
    [[nodiscard]] const T* operator->() const noexcept { return reinterpret_cast<T*>(object); }
    [[nodiscard]] typename std::add_lvalue_reference<T>::type operator*()
    {
        return *reinterpret_cast<T*>(object);
    }
    [[nodiscard]] typename std::add_lvalue_reference<T>::type operator*() const
    {
        return *reinterpret_cast<T*>(object);
    }

    [[nodiscard]] explicit operator bool() const noexcept { return init; }

    void reset()
    {
        if (init) {
            reinterpret_cast<T*>(object)->~T();
        }
        init = false;
    }

    Scoped(Scoped<T>&& other) noexcept : init(other.init)
    {
        if (other.init) {
            std::memcpy(this->object, other.object, sizeof(T));
        }
        other.init = false;
    }
    Scoped& operator=(Scoped<T>&& other) noexcept
    {
        if (init) {
            reinterpret_cast<T*>(object)->~T();
        }
        this->init = other.init;
        if (other.init) {
            std::memcpy(this->object, other.object, sizeof(T));
            other.init = false;
        }
        return *this;
    }

    Scoped(const Scoped<T>&) = delete;
    Scoped& operator=(const Scoped<T>&) = delete;

    ~Scoped() { reset(); }

   private:
    explicit Scoped(bool init) : init(init) {}

    template<typename... Args>
    static Scoped Make(Args&&... args)
    {
        Scoped scoped(true);
        new (scoped.object) T(std::forward<Args>(args)...);
        return scoped;
    }

    template<typename U, typename... Args>
    friend Scoped<U> make_scoped(Args&&...);

   private:
    bool init = false;
    alignas(T) unsigned char object[sizeof(T)];
};

template<typename T, typename... Args>
Scoped<T> make_scoped(Args&&... args)
{
    return Scoped<T>::Make(std::forward<Args>(args)...);
}

}  // namespace firstgame::util

#endif  // FIRSTGAME_UTIL_SCOPED_H_
