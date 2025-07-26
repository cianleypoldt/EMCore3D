#include "io.h"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>

#include <exception>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

namespace io
{

void init_spdlog(spdlog::level::level_enum level) {
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

        spdlog::info("Logger initialized successfully");
    } catch (const std::exception & e) {
        // Fallback to basic console logging
        spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
        spdlog::error("Failed to initialize advanced logging: {}", e.what());
    }
}

void * aligned_malloc(size_t size, size_t align) {
    if (size == 0) {
        return nullptr;
    }

    // Ensure alignment is a power of 2 and >= sizeof(void*)
    if (align < sizeof(void *) || (align & (align - 1)) != 0) {
        spdlog::error("Invalid alignment: {}", align);
        return nullptr;
    }

#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    void * p = nullptr;
    if (posix_memalign(&p, align, size) != 0) {
        spdlog::error("posix_memalign failed for size {} with alignment {}", size, align);
        p = nullptr;
    }
    return p;
#endif
}

void aligned_free(void * p) {
    if (p == nullptr) {
        return;
    }

#ifdef _WIN32
    _aligned_free(p);
#else
    free(p);
#endif
}

bool ensure_directory(const std::string & path) {
    try {
        return fs::create_directories(path);
    } catch (const std::exception & e) {
        spdlog::error("Failed to create directory '{}': {}", path, e.what());
        return false;
    }
}

size_t file_size(const std::string & path) {
    try {
        if (fs::exists(path)) {
            return static_cast<size_t>(fs::file_size(path));
        }
    } catch (const std::exception & e) {
        spdlog::error("Failed to get file size for '{}': {}", path, e.what());
    }
    return 0;
}

bool file_exists(const std::string & path) {
    try {
        return fs::exists(path);
    } catch (const std::exception & e) {
        spdlog::error("Failed to check file existence for '{}': {}", path, e.what());
        return false;
    }
}

}  // namespace io
