#include "yee_grid.h"

YeeGrid::YeeGrid(Eigen::Vector3d dimensions, unsigned int x_resolution)
    : m_single_cube_dim(dimensions[0] / x_resolution) {

    m_resolution[0] = std::round(dimensions[0] / m_single_cube_dim);
    m_resolution[1] = std::round(dimensions[1] / m_single_cube_dim);
    m_resolution[2] = std::round(dimensions[2] / m_single_cube_dim);

    m_dimensions[0] = m_resolution[0] * m_single_cube_dim;
    m_dimensions[1] = m_resolution[1] * m_single_cube_dim;
    m_dimensions[2] = m_resolution[2] * m_single_cube_dim;

    // log: constructing a Yee-grid of {x,y,z} meters using {res_x * res_y * res_z} cubes

    gridAllocate();
}
