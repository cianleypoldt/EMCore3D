#include "yeeGrid.h"
#include <spdlog/spdlog.h>

YeeGrid::YeeGrid(vec3 world_size, R cell_size)
    : m_world_size(world_size) {

    m_grid_dimensions[0] = std::round(world_size[0] / cell_size);
    m_grid_dimensions[1] = std::round(world_size[1] / cell_size);
    m_grid_dimensions[2] = std::round(world_size[2] / cell_size);

    m_cell_size[0] = m_world_size[0] / m_grid_dimensions[0];
    m_cell_size[1] = m_world_size[1] / m_grid_dimensions[1];
    m_cell_size[2] = m_world_size[2] / m_grid_dimensions[2];

    m_cell_count = m_grid_dimensions[0] * m_grid_dimensions[1] * m_grid_dimensions[2];

    spdlog::info("Constructing a grid over {}x{}x{} meters using {} cubes",
                 m_world_size[0], m_world_size[1], m_world_size[2], (int)m_cell_count);
    spdlog::info("Cell dimensions: {}, {}, {}", m_cell_size[0], m_cell_size[1], m_cell_size[2]);

    gridAllocate();
}

YeeGrid::~YeeGrid() {
    gridDeallocate();
}
