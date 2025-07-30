#include "app/simulation.hpp"
#include "common/memory_utils.hpp"
#include "common/types.hpp"
#include "yee/grid.hpp"

#include <spdlog/spdlog.h>

#include <cstring>
#ifdef _WIN32
#    include <malloc.h>
#endif

bool YeeGrid::grid_allocate() {
    if (Simulation::total_heap_allocation + cell_count * memory_per_cell > em_const::memory_cap) {
        spdlog::error("Grid allocation ({} bytes) exceeds memory cap of {}",
                      (int)(cell_count * memory_per_cell),
                      em_const::memory_cap);
        return false;
    }

    grid_allocation = io::aligned_malloc(cell_count * memory_per_cell, 64);

    if (grid_allocation == nullptr) {
        spdlog::error("Grid memory allocation of {} bytes failed", (int)(cell_count * memory_per_cell));
        return false;
    }

    std::memset(grid_allocation, 0, cell_count * memory_per_cell);

    m_Ex = reinterpret_cast<real*>(grid_allocation);
    m_Ey = m_Ex + 1 * cell_count;
    m_Ez = m_Ex + 2 * cell_count;

    m_Hx = m_Ex + 3 * cell_count;
    m_Hy = m_Ex + 4 * cell_count;
    m_Hz = m_Ex + 5 * cell_count;

    inv_permeability = m_Ex + 6 * cell_count;
    inv_permittivity = m_Ex + 7 * cell_count;

    Simulation::total_heap_allocation += cell_count * memory_per_cell;

    spdlog::debug("Grid allocated {} bytes ({:.2f} MB). Total memory in use is {} bytes ({:.2f} MB)",
                  (cell_count * memory_per_cell),
                  (cell_count * memory_per_cell) / (1024.0 * 1024.0),
                  Simulation::total_heap_allocation,
                  Simulation::total_heap_allocation / (1024.0 * 1024.0));

    return true;
}

void YeeGrid::grid_deallocate() {
    if (grid_allocation == nullptr) {
        return;
    }
    io::aligned_free(grid_allocation);
    grid_allocation = nullptr;
    Simulation::total_heap_allocation -= cell_count * memory_per_cell;
    spdlog::debug("Grid deallocated");
}
