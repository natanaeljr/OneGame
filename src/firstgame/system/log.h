/**
 * Logger definitions for game-wide usage.
 */

#ifndef FIRSTGAME_SYSTEM_LOG_H_
#define FIRSTGAME_SYSTEM_LOG_H_

#include <cstdlib>
#include <spdlog/spdlog.h>

#include "firstgame/util/currenton.h"

/**************************************************************************************************/

#define TRACE(...) SPDLOG_LOGGER_TRACE(::firstgame::system::Logger::current().handle(), __VA_ARGS__)
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(::firstgame::system::Logger::current().handle(), __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(::firstgame::system::Logger::current().handle(), __VA_ARGS__)
#define WARN(...) SPDLOG_LOGGER_WARN(::firstgame::system::Logger::current().handle(), __VA_ARGS__)
#define ERROR(...) SPDLOG_LOGGER_ERROR(::firstgame::system::Logger::current().handle(), __VA_ARGS__)
#define CRITICAL(...) \
    SPDLOG_LOGGER_CRITICAL(::firstgame::system::Logger::current().handle(), __VA_ARGS__)

#ifndef NDEBUG
#define ASSERT(cond)                                   \
    do {                                               \
        if (!(cond)) {                                 \
            CRITICAL("Assertion failed: ({})", #cond); \
            abort();                                   \
        }                                              \
    } while (0)
#else
#define ASSERT(cond) (void) 0;
#endif

/**************************************************************************************************/

namespace firstgame::system {

class Logger final : public util::Currenton<Logger> {
   public:
    explicit Logger(std::shared_ptr<spdlog::logger> logger) : logger_(std::move(logger)) {}
    ~Logger() override = default;

    [[nodiscard]] auto& handle() { return logger_; }

   private:
    std::shared_ptr<spdlog::logger> logger_;
};

}  // namespace firstgame::system

#endif  // FIRSTGAME_SYSTEM_LOG_H_