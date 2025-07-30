#pragma once
#include "common/types.hpp"
#include "yee/grid.hpp"

class Simulation
{
  public:
    Simulation(vec3 dimensions);

    void update(int count, const std::function<void(int)>& callback = nullptr);

    void add_particle();
    void set_material();

    static size_t total_heap_allocation;

    real time_step = em_const::default_time_step;

    YeeGrid grid;
};
