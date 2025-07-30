#include "common/spdlog_init.hpp"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

void io::init_spdlog(spdlog::level::level_enum level) {
    try {
        auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console->set_level(level);

        auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/app.log",
                                                                           20 * 1024 * 1024,  // 20 MB
                                                                           5 /*5 rotated files */);
        file->set_level(level);

        spdlog::sinks_init_list sinks{ console, file };
        auto                    logger = std::make_shared<spdlog::logger>("main", sinks);
        logger->set_level(level);

        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");
        spdlog::flush_on(spdlog::level::info);

        // spdlog::info("Logger initialized successfully");
    } catch (const std::exception& e) {
        // Fallback to basic console logging
        spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
        spdlog::error("Failed to initialize advanced logging: {}", e.what());
    }
}
