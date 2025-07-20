#include "../../IO/logger.h"
#include "../emfdtd.h"
#include "yeeGrid.h"

bool YeeGrid::gridAllocate() {

    uint magnetic_field_sample_count = m_grid_dimensions[0] * m_grid_dimensions[1] * m_grid_dimensions[2];
    uint electric_field_sample_count = (m_grid_dimensions[0] + 1) * (m_grid_dimensions[1] + 1) * (m_grid_dimensions[2] + 1);

    m_memory_size = (magnetic_field_sample_count + electric_field_sample_count) * sizeof(vec3);

    if (emftdt::m_total_heap_allocation + m_memory_size > em_const::MEMORY_CAP) {
        printf("Grid allocation (%i bytes) exceeds memory cap of %i.\n", (int)m_memory_size, em_const::MEMORY_CAP);
        return false;
    }

    m_magnetic_field_grid = static_cast<vec3*>(
        Eigen::aligned_allocator<vec3>().allocate(magnetic_field_sample_count + electric_field_sample_count));

    if (!m_magnetic_field_grid) {
        printf("Grid memory allocation of %i was not succesful.\n", (int)m_memory_size);
        return false;
    }

    emftdt::m_total_heap_allocation += m_memory_size;

    printf("Grid allocated %i bytes. Total memory in use is %i bytes.\n", (int)m_memory_size, (int)emftdt::m_total_heap_allocation);

    m_electric_field_grid = m_magnetic_field_grid + magnetic_field_sample_count;

    for (int i = 0; i < magnetic_field_sample_count + electric_field_sample_count; i++) {
        m_magnetic_field_grid[i] = {0, 0, 0};
    }

    for (int z = 0; z < 11; z++) {
        for (int y = 0; y < 11; y++) {
            for (int x = 0; x < 11; x++) {
                auto vec = indexEl(index3{static_cast<uint32_t>(x), static_cast<uint32_t>(y), static_cast<uint32_t>(z)});
                printf("%f, %f, %f\n", vec[0], vec[1], vec[2]);
            }
        }
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
