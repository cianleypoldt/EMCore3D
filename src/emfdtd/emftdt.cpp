#include "emfdtd.h"

size_t emfdtd::TotalHeapAllocation = 0;

emfdtd::emfdtd(vec3 dimensions)
    : grid(dimensions, em_const::default_cube_size) {

    R min_c = std::numeric_limits<R>::max();

    for (int i = 0; i < grid.CellCount; ++i) {
        const R eps = grid.m_InvPermittivity[i];
        const R mu = grid.m_InvPermeability[i];
        const R local_c = 1.0 / std::sqrt(eps * mu);
        if (local_c < min_c) {
            min_c = local_c;
        }
    }
    const R dx = grid.CellSize.maxCoeff();
    time_step = dx / (min_c * std::sqrt(3.0));

    spdlog::info("Maximum stable time step is {}s in the current grid", time_step);
}
