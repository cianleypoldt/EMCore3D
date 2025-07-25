#include "yee_grid.h"
#include <spdlog/spdlog.h>

YeeGrid::YeeGrid(vec3 world_size, R cell_size)
    : WorldSize(world_size) {

    GridDimensions[0] = std::round(world_size[0] / cell_size);
    GridDimensions[1] = std::round(world_size[1] / cell_size);
    GridDimensions[2] = std::round(world_size[2] / cell_size);

    CellSize[0] = WorldSize[0] / GridDimensions[0];
    CellSize[1] = WorldSize[1] / GridDimensions[1];
    CellSize[2] = WorldSize[2] / GridDimensions[2];

    CellCount = GridDimensions[0] * GridDimensions[1] * GridDimensions[2];

    spdlog::info("Constructing a grid over {}x{}x{} meters using {} cubes",
                 WorldSize[0], WorldSize[1], WorldSize[2], (int)CellCount);
    spdlog::info("Cell dimensions: {}, {}, {}", CellSize[0], CellSize[1], CellSize[2]);

    assert(GridAllocate());

    for (int i = 0; i < CellCount; i++) {
        m_InvPermeability[i] = 0.5;
        m_InvPermittivity[i] = 0.5;
    }
}

YeeGrid::~YeeGrid() {
    GridDeallocate();
}
