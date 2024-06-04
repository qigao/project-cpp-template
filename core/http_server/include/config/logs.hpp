#ifndef __LOGS_H__
#define __LOGS_H__
#include "singleton.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <memory>

class Logger : public Singleton<Logger>
{
public:
    ~Logger() { spdlog::shutdown(); }

    std::shared_ptr<spdlog::logger> get()
    {
        if (!logger_)
        {

            spdlog::init_thread_pool(8192, 1);
            auto stdout_sink =
                std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto rotating_sink =
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    "mylog.txt", 1024 * 1024 * 10, 3);
            std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
            logger_ = std::make_shared<spdlog::async_logger>(
                "loggername", sinks.begin(), sinks.end(), spdlog::thread_pool(),
                spdlog::async_overflow_policy::block);
            spdlog::register_logger(logger_);
        }
        return logger_;
    }

private:
    std::shared_ptr<spdlog::logger> logger_;
};

#endif // __LOGS_H__
