#include "../emfdtd.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>
#ifdef _WIN32
#include <malloc.h>
#endif

void* AlignedMalloc(std::size_t size, std::size_t align) {
#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    void* p = nullptr;
    if (posix_memalign(&p, align, size) != 0) p = nullptr;
    return p;
#endif
}

void AlignedFree(void* p) {
#ifdef _WIN32
    _aligned_free(p);
#else
    free(p);
#endif
}

bool yee_grid::GridAllocate() {

    if (emfdtd::TotalHeapAllocation + CellCount * MemoryPerCell > em_const::memory_cap) {
        spdlog::error("Grid allocation ({} bytes) exceeds memory cap of {}", (int)(CellCount * MemoryPerCell), em_const::memory_cap);
        return false;
    }

    GridAllocation = AlignedMalloc(CellCount * MemoryPerCell, 64);

    if (!GridAllocation) {
        spdlog::error("Grid memory allocation of {} bytes failed", (int)(CellCount * MemoryPerCell));
        return false;
    }

    std::memset(GridAllocation, 0, CellCount * MemoryPerCell);

    m_Ex = reinterpret_cast<R*>(GridAllocation);
    m_Ey = m_Ex + 1 * CellCount;
    m_Ez = m_Ex + 2 * CellCount;

    m_Hx = m_Ex + 3 * CellCount;
    m_Hy = m_Ex + 4 * CellCount;
    m_Hz = m_Ex + 5 * CellCount;

    m_InvPermeability = m_Ex + 6 * CellCount;
    m_InvPermittivity = m_Ex + 7 * CellCount;

    emfdtd::TotalHeapAllocation += CellCount * MemoryPerCell;

    spdlog::debug("Grid allocated {} bytes ({:.2f} MB). Total memory in use is {} bytes ({:.2f} MB)",
                  (CellCount * MemoryPerCell),
                  (CellCount * MemoryPerCell) / (1024.0 * 1024.0),
                  emfdtd::TotalHeapAllocation,
                  emfdtd::TotalHeapAllocation / (1024.0 * 1024.0));

    return true;
}

void yee_grid::GridDeallocate() {
    if (!GridAllocation) return;
    AlignedFree(GridAllocation);
    GridAllocation = nullptr;
    emfdtd::TotalHeapAllocation -= CellCount * MemoryPerCell;
    spdlog::debug("Grid deallocated");
}
