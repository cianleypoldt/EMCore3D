#include "../emfdtd.h"
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

    m_Ex = reinterpret_cast<R*>(grid);
    m_Ey = m_Ex + 1 * m_cell_count;
    m_Ez = m_Ex + 2 * m_cell_count;

    m_Hx = m_Ex + 3 * m_cell_count;
    m_Hy = m_Ex + 4 * m_cell_count;
    m_Hz = m_Ex + 5 * m_cell_count;

    m_permeability = m_Ex + 6 * m_cell_count;
    m_permittivity = m_Ex + 7 * m_cell_count;

    emfdtd::m_total_heap_allocation += m_cell_count * MEM_PER_CELL;

    spdlog::debug("Grid allocated {} bytes ({:.2f} MB). Total memory in use is {} bytes ({:.2f} MB)",
                  (m_cell_count * MEM_PER_CELL),
                  (m_cell_count * MEM_PER_CELL) / (1024.0 * 1024.0),
                  emfdtd::m_total_heap_allocation,
                  emfdtd::m_total_heap_allocation / (1024.0 * 1024.0));

    return true;
}

void YeeGrid::gridDeallocate() {
    if (!grid) return;
    aligned_free(grid);
    grid = nullptr;
    emfdtd::m_total_heap_allocation -= m_cell_count * MEM_PER_CELL;
    spdlog::debug("Grid deallocated");
}
