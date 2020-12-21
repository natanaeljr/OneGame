/**
 * Scope utility.
 */

#ifndef FIRSTGAME_UTIL_SCOPED_H_
#define FIRSTGAME_UTIL_SCOPED_H_

namespace firstgame::util {

template<typename T, typename Final = void>
class Scoped;

template<typename T>
class Scoped<T> final {
   public:
    Scoped() = default;

    [[nodiscard]] T& get() noexcept { return reinterpret_cast<T*>(object); }
    [[nodiscard]] const T& get() const noexcept { return reinterpret_cast<T*>(object); }
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

    [[nodiscard]] bool exists() const noexcept { return init; }
    [[nodiscard]] explicit operator bool() const noexcept { return exists(); }

    void reset()
    {
        if (init) {
            reinterpret_cast<T*>(object)->~T();
            init = false;
        }
    }

    Scoped(Scoped<T>&& other) noexcept : init(std::exchange(other.init, false))
    {
        if (init) {
            std::memcpy(this->object, other.object, sizeof(T));
        }
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

    ~Scoped()
    {
        if (init) {
            reinterpret_cast<T*>(object)->~T();
        }
    }

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

template<typename T, typename Final>
class Scoped final {
   public:
    Scoped() = default;

    [[nodiscard]] T& get() noexcept { return *reinterpret_cast<T*>(object); }
    [[nodiscard]] const T& get() const noexcept { return *reinterpret_cast<T*>(object); }
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

    [[nodiscard]] bool exists() const noexcept { return init; }
    [[nodiscard]] explicit operator bool() const noexcept { return exists(); }

    void reset()
    {
        if (init) {
            final_(this->operator*());
            reinterpret_cast<T*>(object)->~T();
            init = false;
        }
    }

    template<typename F>
    explicit Scoped(Scoped<T, F>&& other) noexcept
        : init(std::exchange(other.init, false)), final_(other.final_)
    {
        if (init) {
            std::memcpy(this->object, other.object, sizeof(T));
        }
    }
    template<typename F>
    Scoped& operator=(Scoped<T, F>&& other) & noexcept
    {
        if (init) {
            final_(this->operator*());
            reinterpret_cast<T*>(object)->~T();
        }
        this->init = other.init;
        if (other.init) {
            this->final_ = other.final_;
            std::memcpy(this->object, other.object, sizeof(T));
            other.init = false;
        }
        return *this;
    }

    template<typename F>
    Scoped(const Scoped<T, F>&) = delete;
    template<typename F>
    Scoped& operator=(const Scoped<T, F>&) = delete;

    ~Scoped()
    {
        if (init) {
            final_(this->operator*());
            reinterpret_cast<T*>(object)->~T();
        }
    }

   private:
    template<typename F>
    explicit Scoped(bool init, F&& final) : init(init), final_(std::forward<F>(final))
    {
    }

    template<typename F, typename... Args>
    static Scoped<T, std::decay_t<Final>> Make(F&& final, Args&&... args)
    {
        Scoped<T, std::decay_t<Final>> scoped(true, std::forward<F>(final));
        new (scoped.object) T(std::forward<Args>(args)...);
        return scoped;
    }

    template<typename U, typename F, typename... Args>
    friend Scoped<U, std::decay_t<F>> make_scoped_final(F&&, Args&&...);

   private:
    // this declaration order is important for alignment
    bool init;
    alignas(T) unsigned char object[sizeof(T)];
    Final final_;
};

template<typename T, typename Final, typename... Args>
Scoped<T, std::decay_t<Final>> make_scoped_final(Final&& final, Args&&... args)
{
    return Scoped<T, std::decay_t<Final>>::Make(std::forward<Final>(final),
                                                std::forward<Args>(args)...);
}

}  // namespace firstgame::util

#endif  // FIRSTGAME_UTIL_SCOPED_H_
