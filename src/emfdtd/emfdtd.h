#pragma once
#include "IO/io.h"
#include "src/emfdtd/grid/yee_grid.h"

class emfdtd {
  public:
    emfdtd(vec3 dimensions);

    void update(int count, const std::function<void(int)>& callback = nullptr) {

        if (callback) {
            for (int i = 0; i < count; ++i) {
                grid.updateGridNaive(time_step);
                callback(i);
            }
            return;
        }
        for (int i = 0; i < count; ++i) grid.updateGridNaive(time_step);
    }

    void addParticle();
    void setMaterial();
    static size_t TotalHeapAllocation;

    R time_step = em_const::default_time_step;

    yee_grid grid;
};
