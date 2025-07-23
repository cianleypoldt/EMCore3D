#pragma once
#include "../constants.h"
#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(vec3, R cell_size);
    ~YeeGrid();

    YeeGrid(const YeeGrid&) = delete;
    YeeGrid& operator=(const YeeGrid&) = delete;

    void update(uint step_count, R time_step);
    void updateOMP(uint step_count, R time_step);

    inline R& Ex(index3&);
    inline R& Ey(index3&);
    inline R& Ez(index3&);
    inline R& Hx(index3&);
    inline R& Hy(index3&);
    inline R& Hz(index3&);

    inline R& permeability(index3&);
    inline R& permittivity(index3&);

    static constexpr uint MEM_PER_CELL = 8 * sizeof(R);

    index3 m_grid_dimensions;

    vec3 m_world_size;
    vec3 m_cell_size = vec3(1, 1, 1) * em_const::DEFAULT_CUBE_SIZE;

    uint64_t m_cell_count = 0;

    void* grid = nullptr;

    R *m_Ex, *m_Ey, *m_Ez;
    R *m_Hx, *m_Hy, *m_Hz;

    R* m_permittivity = nullptr;
    R* m_permeability = nullptr;

    bool gridAllocate();
    void gridDeallocate();
};

R& YeeGrid::Ex(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Ey(index3& index) { return m_Ey[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Ez(index3& index) { return m_Ez[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

R& YeeGrid::Hx(index3& index) { return m_Hx[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Hy(index3& index) { return m_Hy[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
R& YeeGrid::Hz(index3& index) { return m_Hz[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

R& YeeGrid::permeability(index3& index) {
    return m_permeability[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
R& YeeGrid::permittivity(index3& index) {
    return m_permittivity[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
