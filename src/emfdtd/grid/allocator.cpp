#include "../emfdtd.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>
#ifdef _WIN32
#include <malloc.h>
#endif

bool yee_grid::grid_allocate() {

    if (emfdtd::TotalHeapAllocation + cell_count * memory_per_cell > em_const::memory_cap) {
        spdlog::error("Grid allocation ({} bytes) exceeds memory cap of {}", (int)(cell_count * memory_per_cell), em_const::memory_cap);
        return false;
    }

    grid_allocation = io::aligned_malloc(cell_count * memory_per_cell, 64);

    if (!grid_allocation) {
        spdlog::error("Grid memory allocation of {} bytes failed", (int)(cell_count * memory_per_cell));
        return false;
    }

    std::memset(grid_allocation, 0, cell_count * memory_per_cell);

    m_Ex = reinterpret_cast<R*>(grid_allocation);
    m_Ey = m_Ex + 1 * cell_count;
    m_Ez = m_Ex + 2 * cell_count;

    m_Hx = m_Ex + 3 * cell_count;
    m_Hy = m_Ex + 4 * cell_count;
    m_Hz = m_Ex + 5 * cell_count;

    inv_permeability = m_Ex + 6 * cell_count;
    inv_permittivity = m_Ex + 7 * cell_count;

    emfdtd::TotalHeapAllocation += cell_count * memory_per_cell;

    spdlog::debug("Grid allocated {} bytes ({:.2f} MB). Total memory in use is {} bytes ({:.2f} MB)",
                  (cell_count * memory_per_cell),
                  (cell_count * memory_per_cell) / (1024.0 * 1024.0),
                  emfdtd::TotalHeapAllocation,
                  emfdtd::TotalHeapAllocation / (1024.0 * 1024.0));

    return true;
}

void yee_grid::grid_deallocate() {
    if (!grid_allocation) return;
    io::aligned_free(grid_allocation);
    grid_allocation = nullptr;
    emfdtd::TotalHeapAllocation -= cell_count * memory_per_cell;
    spdlog::debug("Grid deallocated");
}
