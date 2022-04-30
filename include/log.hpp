#ifndef LOG_H
#define LOG_H

#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace spd = spdlog;

class Log {

    public:
        Log() {
            const std::string loggerName = "spdlog";
            if (useConsole) {
                logger = spdlog::stdout_color_mt(loggerName);
            } else {

            }
        }

        ~Log() {
            spd::shutdown();
        }

        std::shared_ptr<spd::logger> getLogger() {
            return logger;
        }

        void setLogLevel(std::string level) {
            this->level = level;

            if (level == "trace") {
                logger->set_level(spdlog::level::trace);
                logger->flush_on(spdlog::level::trace);
            } else if (level == "debug") {
                logger->set_level(spdlog::level::debug);
                logger->flush_on(spdlog::level::debug);
            } else if (level == "info") {
                logger->set_level(spdlog::level::info);
                logger->flush_on(spdlog::level::info);
            } else if (level == "warn") {
                logger->set_level(spdlog::level::warn);
                logger->flush_on(spdlog::level::warn);
            } else if (level == "error") {
                logger->set_level(spdlog::level::err);
                logger->flush_on(spdlog::level::err);
            } else if (level == "critical") {
                logger->set_level(spdlog::level::critical);
                logger->flush_on(spdlog::level::critical);
            }
        }

        static std::shared_ptr<Log> instance() {
            return Log::log;
        }

    private:
        std::shared_ptr<spd::logger> logger;
        bool useConsole = true;
        static std::shared_ptr<Log> log;
        std::string level = "debug";
};

std::shared_ptr<Log> Log::log = std::shared_ptr<Log>(new Log());

// 有代码行号
#define LOG_TRACE(...)      SPDLOG_LOGGER_CALL(Log::instance()->getLogger().get(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)      SPDLOG_LOGGER_CALL(Log::instance()->getLogger().get(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)       SPDLOG_LOGGER_CALL(Log::instance()->getLogger().get(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)       SPDLOG_LOGGER_CALL(Log::instance()->getLogger().get(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)      SPDLOG_LOGGER_CALL(Log::instance()->getLogger().get(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...)   SPDLOG_LOGGER_CALL(Log::instance()->getLogger().get(), spdlog::level::critical, __VA_ARGS__)

// 没有代码行号
// #define LOG_TRACE(...)       Logger::instance().getLogger().get()->trace(__VA_ARGS__)
// #define LOG_DEBUG(...)       Logger::instance().getLogger().get()->debug(__VA_ARGS__)
// #define LOG_INFO(...)        Logger::instance().getLogger().get()->info(__VA_ARGS__)
// #define LOG_WARN(...)        Logger::instance().getLogger().get()->warn(__VA_ARGS__)
// #define LOG_ERROR(...)       Logger::instance().getLogger().get()->error(__VA_ARGS__)
// #define LOG_CRITICAL(...)    Logger::instance().getLogger().get()->critical(__VA_ARGS__)

#endif