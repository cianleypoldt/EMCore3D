#pragma once
#include "../../common/constants.h"
#include <eigen3/Eigen/Dense>

class yee_grid {

  public:
    yee_grid(vec3, R CellSize);
    ~yee_grid();

    yee_grid(const yee_grid&) = delete;
    yee_grid& operator=(const yee_grid&) = delete;

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

R& yee_grid::Ex(const index3& index) const { return m_Ex[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& yee_grid::Ey(const index3& index) const { return m_Ey[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& yee_grid::Ez(const index3& index) const { return m_Ez[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }

R& yee_grid::Hx(const index3& index) const { return m_Hx[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& yee_grid::Hy(const index3& index) const { return m_Hy[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }
R& yee_grid::Hz(const index3& index) const { return m_Hz[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]]; }

R& yee_grid::InvPermeability(const index3& index) const {
    return m_InvPermeability[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]];
}
R& yee_grid::InvPermittivity(const index3& index) const {
    return m_InvPermittivity[index[0] + index[1] * GridDimensions[0] + index[2] * GridDimensions[0] * GridDimensions[1]];
}
