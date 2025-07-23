#include "emfdtd.h"

size_t emfdtd::m_total_heap_allocation = 0;

emfdtd::emfdtd(vec3 dimensions)
    : grid(dimensions, em_const::DEFAULT_CUBE_SIZE) {

    R min_c = std::numeric_limits<R>::max();

    for (int i = 0; i < grid.m_cell_count; ++i) {
        const R eps = grid.m_permittivity[i];
        const R mu = grid.m_permeability[i];
        const R local_c = 1.0 / std::sqrt(eps * mu);
        if (local_c < min_c) {
            min_c = local_c;
        }
    }
    const R dx = grid.m_cell_size.maxCoeff();
    time_step = dx / (min_c * std::sqrt(3.0));

    spdlog::info("Maximum stable time step is {}s in the current grid", time_step);
}

void ems::initSPDlog(spdlog::level::level_enum level) {
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
