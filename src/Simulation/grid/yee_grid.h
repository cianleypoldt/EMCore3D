#pragma once
#include "../constants.h"
#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(vec3, R CellSize);
    ~YeeGrid();

    YeeGrid(const YeeGrid&) = delete;
    YeeGrid& operator=(const YeeGrid&) = delete;

    void updateGridNaive(R TimeStep);

    inline R& Ex(const index3&) const;
    inline R& Ey(const index3&) const;
    inline R& Ez(const index3&) const;
    inline R& Hx(const index3&) const;
    inline R& Hy(const index3&) const;
    inline R& Hz(const index3&) const;

    inline R& InvPermeability(const index3&) const;
    inline R& InvPermittivity(const index3&) const;

    static constexpr uint MemoryPerCell = 8 * sizeof(R);

    index3 GridDimensions;

    vec3 WorldSize;
    vec3 CellSize = vec3(1, 1, 1) * em_const::DEFAULT_CUBE_SIZE;

    uint64_t CellCount = 0;

    void* GridAllocation = nullptr;

    R *m_Ex, *m_Ey, *m_Ez;
    R *m_Hx, *m_Hy, *m_Hz;

    R* m_InvPermittivity = nullptr;
    R* m_InvPermeability = nullptr;

    bool GridAllocate();
    void GridDeallocate();
};

R& YeeGrid::Ex(const index3& index) const { return m_Ex[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& YeeGrid::Ey(const index3& index) const { return m_Ey[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& YeeGrid::Ez(const index3& index) const { return m_Ez[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }

R& YeeGrid::Hx(const index3& index) const { return m_Hx[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& YeeGrid::Hy(const index3& index) const { return m_Hy[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& YeeGrid::Hz(const index3& index) const { return m_Hz[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }

R& YeeGrid::InvPermeability(const index3& index) const {
    return m_InvPermeability[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]];
}
R& YeeGrid::InvPermittivity(const index3& index) const {
    return m_InvPermittivity[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]];
}
