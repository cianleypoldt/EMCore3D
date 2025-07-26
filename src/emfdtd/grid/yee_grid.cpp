#include "emfdtd/grid/yee_grid.h"
#include <spdlog/spdlog.h>

yee_grid::yee_grid(vec3 world_size, R cell_size)
    : world_size(world_size) {

    grid_dimensions[0] = std::round(world_size[0] / cell_size);
    grid_dimensions[1] = std::round(world_size[1] / cell_size);
    grid_dimensions[2] = std::round(world_size[2] / cell_size);

    cell_dimensions[0] = world_size[0] / grid_dimensions[0];
    cell_dimensions[1] = world_size[1] / grid_dimensions[1];
    cell_dimensions[2] = world_size[2] / grid_dimensions[2];

    cell_count = grid_dimensions[0] * grid_dimensions[1] * grid_dimensions[2];

    spdlog::info("Constructing a grid over {}x{}x{} meters using {} cubes",
                 world_size[0], world_size[1], world_size[2], (int)cell_count);
    spdlog::info("Cell dimensions: {}, {}, {}", cell_dimensions[0], cell_dimensions[1], cell_dimensions[2]);

    assert(grid_allocate());

    for (int i = 0; i < cell_count; i++) {
        inv_permeability[i] = 0.5;
        inv_permittivity[i] = 0.5;
    }
}

yee_grid::~yee_grid() {
    grid_deallocate();
}
