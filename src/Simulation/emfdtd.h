#pragma once
#include "grid/yee_grid.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class emfdtd {
  public:
    emfdtd(vec3 dimensions);

    void update(uint count) {
        for (int i = 0; i < count; ++i) grid.updateGridNaive(count);
    }

    void addParticle();
    void setMaterial();
    static size_t TotalHeapAllocation;

    R time_step = em_const::DEFAULT_TIME_STEP;

  private:
    YeeGrid grid;
};

namespace ems {
void initSPDlog(spdlog::level::level_enum level);
}
