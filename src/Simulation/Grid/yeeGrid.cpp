#include "yeeGrid.h"
#include "../../IO/logger.h"

YeeGrid::YeeGrid(vec3 world_size, double cell_size)
    : m_cell_size(cell_size) {

    m_grid_dimensions[0] = std::round(world_size[0] / m_cell_size);
    m_grid_dimensions[1] = std::round(world_size[1] / m_cell_size);
    m_grid_dimensions[2] = std::round(world_size[2] / m_cell_size);

    m_world_size[0] = m_grid_dimensions[0] * m_cell_size;
    m_world_size[1] = m_grid_dimensions[1] * m_cell_size;
    m_world_size[2] = m_grid_dimensions[2] * m_cell_size;

    printf("Constructing a grid over {%f,%f,%f} meters using %i cubes.\n", m_world_size[0], m_world_size[1], m_world_size[2], m_grid_dimensions[0] * m_grid_dimensions[1] * m_grid_dimensions[2]);
    // log: cell size: {m_cell_size}.

    gridAllocate();
}

YeeGrid::~YeeGrid() {
    gridDeallocate();
}

vec3& YeeGrid::indexMag(index3 index) {
    return m_magnetic_field_grid[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
vec3& YeeGrid::indexEl(index3 index) {
    return m_electric_field_grid[index[0] + index[1] * (m_grid_dimensions[0] + 1) + index[2] * (m_grid_dimensions[0] + 1) * (m_grid_dimensions[1] + 1)];
}
