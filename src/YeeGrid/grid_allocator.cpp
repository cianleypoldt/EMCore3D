#include "yee_grid.h"

bool YeeGrid::gridAllocate() {

    uint32_t magnetic_field_sample_count = m_resolution[0] * m_resolution[1] * m_resolution[2];
    uint32_t electric_field_sample_count = (m_resolution[0] + 1) * (m_resolution[1] + 1) * (m_resolution[2] + 1);

    // 3 vector components, each double precision float

    m_memory = static_cast<Eigen::Vector3d*>(
        Eigen::aligned_allocator<Eigen::Vector3d>().allocate(magnetic_field_sample_count + electric_field_sample_count));
    if (!m_memory) return false;
    m_memory_size = (magnetic_field_sample_count + electric_field_sample_count) * sizeof(Eigen::Vector3d);
    // log
    return true;
}
