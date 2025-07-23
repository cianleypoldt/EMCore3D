#pragma once
#include "Grid/yeeGrid.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class emfdtd {
  public:
    emfdtd(vec3 dimensions);

    void update(uint count) { grid.updateOMP(count, time_step); }

    void addParticle();
    void setMaterial();
    static size_t m_total_heap_allocation;

    R time_step = em_const::DEFAULT_TIME_STEP;

  private:
    YeeGrid grid;
};

namespace ems {
void initSPDlog(spdlog::level::level_enum level);
}
