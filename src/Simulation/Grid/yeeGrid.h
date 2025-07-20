#pragma once
#include "../constants.h"
#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(vec3, double cell_size);
    ~YeeGrid();

    vec3& indexMag(index3 index);
    vec3& indexEl(index3 index);

  private:
    index3 m_grid_dimensions;
    vec3 m_world_size;
    double m_cell_size;

    size_t m_memory_size;

    uint m_magnetic_sample_count = 0;
    uint m_electric_sample_count = 0;
    vec3* m_magnetic_field_grid = nullptr;
    vec3* m_electric_field_grid = nullptr;

    bool gridAllocate();
    void gridDeallocate();
};
