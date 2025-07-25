#include "yee_grid.h"

void yee_grid::updateGridNaive(R TimeStep) {

    // Amperes law
    for (int k = 0; k < GridDimensions[2]; k++) {
        for (int j = 0; j < GridDimensions[1]; j++) {
            for (int i = 0; i < GridDimensions[0]; i++) {

                if (j != 0 && k != 0)
                    Ex({i, j, k}) += TimeStep * InvPermittivity({i, j, k}) *
                                     ((Hz({i, j, k}) - Hz({i, j, k - 1})) / CellSize[1] - (Hy({i, j, k}) - Hy({i, j - 1, k})) / CellSize[2]);
                if (i != 0 && k != 0)
                    Ey({i, j, k}) += TimeStep * InvPermittivity({i, j, k}) *
                                     ((Hx({i - 1, j, k}) - Hx({i, j, k})) / CellSize[2] - (Hz({i, j, k - 1}) - Hz({i, j, k})) / CellSize[0]);
                if (i != 0 && j != 0)
                    Ez({i, j, k}) += TimeStep * InvPermittivity({i, j, k}) *
                                     ((Hy({i, j - 1, k}) - Hy({i, j, k})) / CellSize[0] - (Hx({i - 1, j, k}) - Hx({i, j, k})) / CellSize[1]);
            }
        }
    }
    // Faraday's law
    for (int k = 0; k < GridDimensions[2]; k++) {
        for (int j = 0; j < GridDimensions[1]; j++) {
            for (int i = 0; i < GridDimensions[0]; i++) {

                if (j != GridDimensions[1] - 1 && k != GridDimensions[2] - 1)
                    Hx({i, j, k}) -= TimeStep * InvPermeability({i, j, k}) *
                                     ((Ez({i, j + 1, k}) - Ez({i, j, k})) / CellSize[1] - (Ey({i, j, k + 1}) - Ey({i, j, k})) / CellSize[2]);
                if (i != GridDimensions[0] - 1 && k != GridDimensions[2] - 1)
                    Hy({i, j, k}) -= TimeStep * InvPermeability({i, j, k}) *
                                     ((Ex({i, j, k + 1}) - Ex({i, j, k})) / CellSize[2] - (Ez({i + 1, j, k}) - Ez({i, j, k})) / CellSize[0]);
                if (i != GridDimensions[0] - 1 && j != GridDimensions[1] - 1)
                    Hz({i, j, k}) -= TimeStep * InvPermeability({i, j, k}) *
                                     ((Ey({i + 1, j, k}) - Ey({i, j, k})) / CellSize[0] - (Ex({i, j + 1, k}) - Ex({i, j, k})) / CellSize[1]);
            }
        }
    }
}
