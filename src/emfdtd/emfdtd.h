#pragma once
#include "common/constants.h"
#include "src/emfdtd/grid/yee_grid.h"
#include <functional>
#include <cstddef>

class Emfdtd
{
  public:
    Emfdtd(vec3 dimensions);

    void update(int count, const std::function<void(int)> & callback = nullptr) {
        if (callback) {
            for (int i = 0; i < count; ++i) {
                grid.naive_grid_update(time_step);
                callback(i);
            }
            return;
        }
        for (int i = 0; i < count; ++i) {
            grid.naive_grid_update(time_step);
        }
    }

    void add_particle();
    void set_material();

    static size_t total_heap_allocation;

    real time_step = em_const::default_time_step;

    YeeGrid grid;
};
