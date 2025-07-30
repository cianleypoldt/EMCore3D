#include "app/simulation.hpp"

#include "common/constants.hpp"

#include <spdlog/spdlog.h>

size_t Simulation::total_heap_allocation = 0;

Simulation::Simulation(vec3 dimensions) :
    grid(std::move(dimensions), em_const::default_cube_size) {
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

void Simulation::update(int count, const std::function<void(int)>& callback) {
    if (callback) {
        for (int i = 0; i < count; ++i) {
            grid.naive_grid_update(time_step);
            callback(i);
        }
        return;
    }
    for (int i = 0; i < count; ++i) {
        grid.naive_grid_update(time_step);
    }
}
