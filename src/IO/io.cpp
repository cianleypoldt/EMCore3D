#include "io.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

void io::init_spdlog(spdlog::level::level_enum level) {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "app.log", 20 * 1024 * 1024, 3);

    spdlog::sinks_init_list sinks{console, file};
    auto logger = std::make_shared<spdlog::logger>("main", sinks);
    spdlog::set_default_logger(logger);

    spdlog::set_level(level);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
    spdlog::flush_on(spdlog::level::info);
}

void* io::aligned_malloc(size_t size, size_t align) {
#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    void* p = nullptr;
    if (posix_memalign(&p, align, size) != 0) p = nullptr;
    return p;
#endif
}

void io::aligned_free(void* p) {
#ifdef _WIN32
    _aligned_free(p);
#else
    free(p);
#endif
}
