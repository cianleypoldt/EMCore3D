#pragma once
#include "../../IO/io.h"
#include <eigen3/Eigen/Dense>

class yee_grid {

  public:
    yee_grid(vec3, R CellSize);
    ~yee_grid();

    yee_grid(const yee_grid&) = delete;
    yee_grid& operator=(const yee_grid&) = delete;

    void updateGridNaive(R TimeStep);

    inline R& Ex(const index3&) const;
    inline R& Ey(const index3&) const;
    inline R& Ez(const index3&) const;
    inline R& Hx(const index3&) const;
    inline R& Hy(const index3&) const;
    inline R& Hz(const index3&) const;

    inline R& get_inv_permeability(const index3&) const;
    inline R& get_inv_permittivity(const index3&) const;

    static constexpr int memory_per_cell = 8 * sizeof(R);

    index3 grid_dimensions;

    vec3 world_size;
    vec3 cell_dimensions = vec3(1, 1, 1) * em_const::default_cube_size;

    uint64_t cell_count = 0;

    void* grid_allocation = nullptr;

    R *m_Ex, *m_Ey, *m_Ez;
    R *m_Hx, *m_Hy, *m_Hz;

    R* inv_permittivity = nullptr;
    R* inv_permeability = nullptr;

    bool grid_allocate();
    void grid_deallocate();
};

R& yee_grid::Ex(const index3& index) const { return m_Ex[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]]; }
R& yee_grid::Ey(const index3& index) const { return m_Ey[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]]; }
R& yee_grid::Ez(const index3& index) const { return m_Ez[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]]; }

R& yee_grid::Hx(const index3& index) const { return m_Hx[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]]; }
R& yee_grid::Hy(const index3& index) const { return m_Hy[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]]; }
R& yee_grid::Hz(const index3& index) const { return m_Hz[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]]; }

R& yee_grid::get_inv_permeability(const index3& index) const {
    return inv_permeability[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]];
}
R& yee_grid::get_inv_permittivity(const index3& index) const {
    return inv_permittivity[index[0] + index[1] * grid_dimensions[0] + index[2] * grid_dimensions[0] * grid_dimensions[1]];
}
