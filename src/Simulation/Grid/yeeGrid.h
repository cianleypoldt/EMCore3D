#pragma once
#include "../constants.h"
#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(vec3, double cell_size);
    ~YeeGrid();

    vec3& indexMag(index3 index);
    vec3& indexEl(index3 index);

    YeeGrid(const YeeGrid&) = delete;
    YeeGrid& operator=(const YeeGrid&) = delete;

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

    double* permittivity;
    double* permeability;

    bool gridAllocate();
    void gridDeallocate();
};
