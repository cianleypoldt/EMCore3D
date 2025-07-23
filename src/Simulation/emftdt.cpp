#include "emfdtd.h"

size_t emfdtd::m_total_heap_allocation = 0;

emfdtd::emfdtd(vec3 dimensions)
    : grid(dimensions, em_const::DEFAULT_CUBE_SIZE) {
}
