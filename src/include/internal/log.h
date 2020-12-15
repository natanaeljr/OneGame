#ifndef FIRSTGAME_INTERNAL_LOG_H_
#define FIRSTGAME_INTERNAL_LOG_H_

#include <spdlog/spdlog.h>
#include <cstdlib>

#define TRACE(...) SPDLOG_LOGGER_TRACE(::firstgame::g_logger, __VA_ARGS__)
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(::firstgame::g_logger, __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(::firstgame::g_logger, __VA_ARGS__)
#define WARN(...) SPDLOG_LOGGER_WARN(::firstgame::g_logger, __VA_ARGS__)
#define ERROR(...) SPDLOG_LOGGER_ERROR(::firstgame::g_logger, __VA_ARGS__)
#define TRACE(...) SPDLOG_LOGGER_TRACE(::firstgame::g_logger, __VA_ARGS__)
#define CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::firstgame::g_logger, __VA_ARGS__)

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

namespace firstgame {

//! Global FirstGame logger
std::shared_ptr<spdlog::logger> g_logger;

}  // namespace firstgame

#endif  // FIRSTGAME_INTERNAL_LOG_H_