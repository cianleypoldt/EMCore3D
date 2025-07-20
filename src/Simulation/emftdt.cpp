#include "emfdtd.h"

size_t emftdt::m_total_heap_allocation = 0;

emftdt::emftdt(vec3 dimensions)
    : grid(dimensions, em_const::DEFAULT_CUBE_SIZE) {
}
