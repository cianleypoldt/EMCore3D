#pragma once
#include "common/constants.hpp"

#include <cstdint>

class YeeGrid
{

  public:
    YeeGrid(vec3 world_size, real cell_size);
    ~YeeGrid();

    YeeGrid(const YeeGrid&)            = delete;
    YeeGrid& operator=(const YeeGrid&) = delete;

    void naive_grid_update(real time_step);

    [[nodiscard]] inline real& ex(const index3& index) const;
    [[nodiscard]] inline real& ey(const index3& index) const;
    [[nodiscard]] inline real& ez(const index3& index) const;
    [[nodiscard]] inline real& hx(const index3& index) const;
    [[nodiscard]] inline real& hy(const index3& index) const;
    [[nodiscard]] inline real& hz(const index3& index) const;

    [[nodiscard]] inline real& get_inv_permeability(const index3& index) const;
    [[nodiscard]] inline real& get_inv_permittivity(const index3& index) const;

    static constexpr int memory_per_cell = 8 * sizeof(real);

    index3 grid_dimensions;

    vec3 world_size;
    vec3 cell_dimensions = vec3(1, 1, 1) * em_const::default_cube_size;

    uint64_t cell_count = 0;

    void* grid_allocation = nullptr;

    real *m_Ex, *m_Ey, *m_Ez;
    real *m_Hx, *m_Hy, *m_Hz;

    real* inv_permittivity = nullptr;
    real* inv_permeability = nullptr;

    bool grid_allocate();
    void grid_deallocate();
};

real& YeeGrid::ex(const index3& index) const {
    return m_Ex[index[0] + (index[1] * grid_dimensions[0]) + (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::ey(const index3& index) const {
    return m_Ey[index[0] + (index[1] * grid_dimensions[0]) + (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::ez(const index3& index) const {
    return m_Ez[index[0] + (index[1] * grid_dimensions[0]) + (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::hx(const index3& index) const {
    return m_Hx[index[0] + (index[1] * grid_dimensions[0]) + (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::hy(const index3& index) const {
    return m_Hy[index[0] + (index[1] * grid_dimensions[0]) + (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::hz(const index3& index) const {
    return m_Hz[index[0] + (index[1] * grid_dimensions[0]) + (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::get_inv_permeability(const index3& index) const {
    return inv_permeability[index[0] + (index[1] * grid_dimensions[0]) +
                            (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}

real& YeeGrid::get_inv_permittivity(const index3& index) const {
    return inv_permittivity[index[0] + (index[1] * grid_dimensions[0]) +
                            (index[2] * grid_dimensions[0] * grid_dimensions[1])];
}
