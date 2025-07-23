#pragma once
#include "../constants.h"
#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(vec3, R cell_size);
    ~YeeGrid();

    YeeGrid(const YeeGrid&) = delete;
    YeeGrid& operator=(const YeeGrid&) = delete;

    inline R& Ex_a(index3&);
    inline R& Ey_a(index3&);
    inline R& Ez_a(index3&);
    inline R& Mx_a(index3&);
    inline R& My_a(index3&);
    inline R& Mz_a(index3&);

    inline R& Ex_b(index3&);
    inline R& Ey_b(index3&);
    inline R& Ez_b(index3&);
    inline R& Mx_b(index3&);
    inline R& My_b(index3&);
    inline R& Mz_b(index3&);

    inline R& permeability(index3&);
    inline R& permittivity(index3&);

  private:
    static constexpr uint MEM_PER_CELL = 8 * sizeof(R) * 2; // 8 real numbers * 1 buffer + 1 grid

    index3 m_grid_dimensions;

    vec3 m_world_size;
    vec3 m_cell_size = vec3(1, 1, 1) * em_const::DEFAULT_CUBE_SIZE;

    uint64_t m_cell_count = 0;

    void* grid;

    R *m_Ex_a, *m_Ey_a, *m_Ez_a;
    R *m_Ex_b, *m_Ey_b, *m_Ez_b;
    R *m_Mx_a, *m_My_a, *m_Mz_a;
    R *m_Mx_b, *m_My_b, *m_Mz_b;

    R* m_permittivity;
    R* m_permeability;

    bool gridAllocate();
    void gridDeallocate();
};

R& YeeGrid::Ex_a(index3& index) { return m_Ex_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Ey_a(index3& index) { return m_Ey_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Ez_a(index3& index) { return m_Ez_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

R& YeeGrid::Mx_a(index3& index) { return m_Mx_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::My_a(index3& index) { return m_My_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Mz_a(index3& index) { return m_Mz_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

R& YeeGrid::Ex_b(index3& index) { return m_Ex_b[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Ey_b(index3& index) { return m_Ey_b[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Ez_b(index3& index) { return m_Ez_b[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

R& YeeGrid::Mx_b(index3& index) { return m_Mx_b[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::My_b(index3& index) { return m_My_b[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Mz_b(index3& index) { return m_Mz_b[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

R& YeeGrid::permeability(index3& index) {
    return m_Ex_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
R& YeeGrid::permittivity(index3& index) {
    return m_Ex_a[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
