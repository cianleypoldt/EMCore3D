#pragma once
#include "../constants.h"
#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(vec3, double cell_size);
    ~YeeGrid();

    YeeGrid(const YeeGrid&) = delete;
    YeeGrid& operator=(const YeeGrid&) = delete;

    inline double& Ex(index3&);
    inline double& Ey(index3&);
    inline double& Ez(index3&);
    inline double& Mx(index3&);
    inline double& My(index3&);
    inline double& Mz(index3&);
    inline double& permeability(index3&);
    inline double& permittivity(index3&);

  private:
    static constexpr uint MEM_PER_CELL = 8 * sizeof(double);

    index3 m_grid_dimensions;

    vec3 m_world_size;
    vec3 m_cell_size = vec3(1, 1, 1) * em_const::DEFAULT_CUBE_SIZE;

    uint64_t m_cell_count = 0;

    void* grid;

    double* m_Ex;
    double* m_Ey;
    double* m_Ez;

    double* m_Mx;
    double* m_My;
    double* m_Mz;

    double* m_permittivity;
    double* m_permeability;

    bool gridAllocate();
    void gridDeallocate();
};

double& YeeGrid::Ex(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
double& YeeGrid::Ey(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
double& YeeGrid::Ez(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

double& YeeGrid::Mx(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
double& YeeGrid::My(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }
double& YeeGrid::Mz(index3& index) { return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]]; }

double& YeeGrid::permeability(index3& index) {
    return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
double& YeeGrid::permittivity(index3& index) {
    return m_Ex[index[0] + index[1] * m_grid_dimensions[0] + index[2] * m_grid_dimensions[0] * m_grid_dimensions[1]];
}
