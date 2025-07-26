#include "common/constants.h"
#include "emfdtd.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstddef>
#include <utility>
#include <limits>
#include <cmath>

size_t Emfdtd::total_heap_allocation = 0;

Emfdtd::Emfdtd(vec3 dimensions) : grid(std::move(dimensions), em_const::default_cube_size) {
    real min_c = std::numeric_limits<real>::max();

    for (int i = 0; i < grid.cell_count; ++i) {
        const real eps     = grid.inv_permittivity[i];
        const real mu      = grid.inv_permeability[i];
        const real local_c = 1.0 / std::sqrt(eps * mu);
        min_c              = std::min(local_c, min_c);
    }
    const real dx = grid.cell_dimensions.maxCoeff();
    time_step     = dx / (min_c * std::sqrt(3.0));

    spdlog::info("Maximum stable time step is {}s in the current grid", time_step);
}
