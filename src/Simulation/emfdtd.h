#pragma once
#include "Grid/yeeGrid.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class emfdtd {
  public:
    emfdtd(vec3 dimensions);

    void addParticle();
    void setMaterial();
    static size_t m_total_heap_allocation;

  private:
    YeeGrid grid;
};

void initSPDlog() {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "app.log", 20 * 1024 * 1024, 3);

    spdlog::sinks_init_list sinks{console, file};
    auto logger = std::make_shared<spdlog::logger>("main", sinks);
    spdlog::set_default_logger(logger);

    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
    spdlog::flush_on(spdlog::level::info);
}
