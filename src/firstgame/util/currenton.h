#ifndef FIRSTGAME_UTIL_CURRENTON_H_
#define FIRSTGAME_UTIL_CURRENTON_H_

#include <cassert>

namespace firstgame::util {

template<class C>
class Currenton {
   public:
    static C& current()
    {
        assert(s_current_ != nullptr);
        return *static_cast<C*>(s_current_);
    }

   protected:
    Currenton() { s_current_ = this; }
    virtual ~Currenton()
    {
        if (s_current_ == this) {
            s_current_ = nullptr;
        }
    }

   private:
    static Currenton<C>* s_current_;
};

template<class C>
Currenton<C>* Currenton<C>::s_current_ = nullptr;

}  // namespace firstgame::util

#endif  // FIRSTGAME_UTIL_CURRENTON_H_
