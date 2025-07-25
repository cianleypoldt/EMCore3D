#pragma once
#include "../IO/io.h"
#include "grid/yee_grid.h"

class emfdtd {
  public:
    emfdtd(vec3 dimensions);

    void update(uint count) {
        for (int i = 0; i < count; ++i) grid.updateGridNaive(count);
    }

    void addParticle();
    void setMaterial();
    static size_t TotalHeapAllocation;

    R time_step = em_const::default_time_step;

  private:
    yee_grid grid;
};
