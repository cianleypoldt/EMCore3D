#include "../../IO/logger.h"
#include "../emfdtd.h"
#include "yeeGrid.h"

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

    if (emftdt::m_total_heap_allocation + m_cell_count * MEM_PER_CELL > em_const::MEMORY_CAP) {
        printf("Grid allocation (%i bytes) exceeds memory cap of %i.\n", (int)(m_cell_count * MEM_PER_CELL), em_const::MEMORY_CAP);
        return false;
    }

    grid = aligned_malloc(64, m_cell_count * MEM_PER_CELL);

    if (!grid) {
        printf("Grid memory allocation of %i bytes was not successful.\n", (int)(m_cell_count * MEM_PER_CELL));
        return false;
    }

    std::memset(grid, 0, m_cell_count * MEM_PER_CELL);

    m_Ex = reinterpret_cast<double*>(grid);
    m_Ey = m_Ex + m_cell_count;
    m_Ez = m_Ex + 2 * m_cell_count;

    m_Mx = m_Ex + 3 * m_cell_count;
    m_My = m_Ex + 4 * m_cell_count;
    m_Mz = m_Ex + 5 * m_cell_count;

    emftdt::m_total_heap_allocation += m_cell_count * MEM_PER_CELL;

    printf("Grid allocated %i bytes. Total memory in use is %i bytes.\n", (int)(m_cell_count * MEM_PER_CELL), (int)emftdt::m_total_heap_allocation);

    return true;
}

void YeeGrid::gridDeallocate() {
    if (!grid) return;
    aligned_free(grid);
    grid = nullptr;
    emftdt::m_total_heap_allocation -= m_cell_count * MEM_PER_CELL;
    printf("Grid deallocated.\n");
}
