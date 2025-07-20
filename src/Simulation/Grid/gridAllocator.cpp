#include "../../IO/logger.h"
#include "../emfdtd.h"
#include "yeeGrid.h"

bool YeeGrid::gridAllocate() {

    uint m_magnetic_sample_count = m_grid_dimensions[0] * m_grid_dimensions[1] * m_grid_dimensions[2];
    uint m_electric_sample_count = (m_grid_dimensions[0] + 1) * (m_grid_dimensions[1] + 1) * (m_grid_dimensions[2] + 1);

    m_memory_size = (m_magnetic_sample_count + m_electric_sample_count) * sizeof(vec3);

    if (emftdt::m_total_heap_allocation + m_memory_size > em_const::MEMORY_CAP) {
        printf("Grid allocation (%i bytes) exceeds memory cap of %i.\n", (int)m_memory_size, em_const::MEMORY_CAP);
        return false;
    }

    m_magnetic_field_grid = static_cast<vec3*>(
        Eigen::aligned_allocator<vec3>().allocate(m_magnetic_sample_count + m_electric_sample_count));

    if (!m_magnetic_field_grid) {
        printf("Grid memory allocation of %i was not successful.\n", (int)m_memory_size);
        return false;
    }

    emftdt::m_total_heap_allocation += m_memory_size;

    printf("Grid allocated %i bytes. Total memory in use is %i bytes.\n", (int)m_memory_size, (int)emftdt::m_total_heap_allocation);

    m_electric_field_grid = m_magnetic_field_grid + m_magnetic_sample_count;

    for (int i = 0; i < m_magnetic_sample_count + m_electric_sample_count; i++) {
        m_magnetic_field_grid[i] = {0, 0, 0};
    }

    return true;
}

void YeeGrid::gridDeallocate() {
    if (!m_magnetic_field_grid) return;
    Eigen::aligned_allocator<vec3>().destroy(m_magnetic_field_grid);
    m_magnetic_field_grid = nullptr;
    m_electric_field_grid = nullptr;
    emftdt::m_total_heap_allocation -= m_memory_size;
    printf("Grid deallocated.\n");
}
