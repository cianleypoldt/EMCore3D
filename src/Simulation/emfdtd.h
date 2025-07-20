#pragma once
#include "Grid/yeeGrid.h"

class emftdt {
  public:
    emftdt(vec3 dimensions);

    void addParticle();
    void setMaterial();
    static size_t m_total_heap_allocation;

  private:
    YeeGrid grid;
};
