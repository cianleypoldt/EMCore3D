#include "../emfdtd.h"
#include "yeeGrid.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>
#ifdef _WIN32
#include <malloc.h>
#endif

void* aligned_malloc(std::size_t size, std::size_t align) {
#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    void* p = nullptr;
    if (posix_memalign(&p, align, size) != 0) p = nullptr;
    return p;
#endif
}

void aligned_free(void* p) {
#ifdef _WIN32
    _aligned_free(p);
#else
    free(p);
#endif
}

bool YeeGrid::gridAllocate() {

    if (emfdtd::m_total_heap_allocation + m_cell_count * MEM_PER_CELL > em_const::MEMORY_CAP) {
        spdlog::error("Grid allocation ({} bytes) exceeds memory cap of {}", (int)(m_cell_count * MEM_PER_CELL), em_const::MEMORY_CAP);
        return false;
    }

    grid = aligned_malloc(m_cell_count * MEM_PER_CELL, 64);

    if (!grid) {
        spdlog::error("Grid memory allocation of {} bytes failed", (int)(m_cell_count * MEM_PER_CELL));
        return false;
    }

    std::memset(grid, 0, m_cell_count * MEM_PER_CELL);

    m_Ex_a = reinterpret_cast<R*>(grid);
    m_Ey_a = m_Ex_a + 1 * m_cell_count;
    m_Ez_a = m_Ex_a + 2 * m_cell_count;

    m_Mx_a = m_Ex_a + 3 * m_cell_count;
    m_My_a = m_Ex_a + 4 * m_cell_count;
    m_Mz_a = m_Ex_a + 5 * m_cell_count;

    m_Ex_b = m_Ex_a + 6 * m_cell_count;
    m_Ey_b = m_Ex_a + 7 * m_cell_count;
    m_Ez_b = m_Ex_a + 8 * m_cell_count;

    m_Mx_b = m_Ex_a + 9 * m_cell_count;
    m_My_b = m_Ex_a + 10 * m_cell_count;
    m_Mz_b = m_Ex_a + 11 * m_cell_count;

    emfdtd::m_total_heap_allocation += m_cell_count * MEM_PER_CELL;

    spdlog::debug("Grid allocated {} bytes. Total memory in use is {} bytes", (m_cell_count * MEM_PER_CELL), emfdtd::m_total_heap_allocation);

    return true;
}

void YeeGrid::gridDeallocate() {
    if (!grid) return;
    aligned_free(grid);
    grid = nullptr;
    emfdtd::m_total_heap_allocation -= m_cell_count * MEM_PER_CELL;
    spdlog::debug("Grid deallocated");
}
