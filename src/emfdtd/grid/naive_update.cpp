#include "emfdtd/grid/yee_grid.h"

void yee_grid::updateGridNaive(R TimeStep) {

    // Amperes law
    for (int k = 0; k < grid_dimensions[2]; k++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {
            for (int i = 0; i < grid_dimensions[0]; i++) {

                if (j != 0 && k != 0)
                    Ex({i, j, k}) += TimeStep * get_inv_permittivity({i, j, k}) *
                                     ((Hz({i, j, k}) - Hz({i, j, k - 1})) / cell_dimensions[1] - (Hy({i, j, k}) - Hy({i, j - 1, k})) / cell_dimensions[2]);
                if (i != 0 && k != 0)
                    Ey({i, j, k}) += TimeStep * get_inv_permittivity({i, j, k}) *
                                     ((Hx({i - 1, j, k}) - Hx({i, j, k})) / cell_dimensions[2] - (Hz({i, j, k - 1}) - Hz({i, j, k})) / cell_dimensions[0]);
                if (i != 0 && j != 0)
                    Ez({i, j, k}) += TimeStep * get_inv_permittivity({i, j, k}) *
                                     ((Hy({i, j - 1, k}) - Hy({i, j, k})) / cell_dimensions[0] - (Hx({i - 1, j, k}) - Hx({i, j, k})) / cell_dimensions[1]);
            }
        }
    }
    // Faraday's law
    for (int k = 0; k < grid_dimensions[2]; k++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {
            for (int i = 0; i < grid_dimensions[0]; i++) {

                if (j != grid_dimensions[1] - 1 && k != grid_dimensions[2] - 1)
                    Hx({i, j, k}) -= TimeStep * get_inv_permeability({i, j, k}) *
                                     ((Ez({i, j + 1, k}) - Ez({i, j, k})) / cell_dimensions[1] - (Ey({i, j, k + 1}) - Ey({i, j, k})) / cell_dimensions[2]);
                if (i != grid_dimensions[0] - 1 && k != grid_dimensions[2] - 1)
                    Hy({i, j, k}) -= TimeStep * get_inv_permeability({i, j, k}) *
                                     ((Ex({i, j, k + 1}) - Ex({i, j, k})) / cell_dimensions[2] - (Ez({i + 1, j, k}) - Ez({i, j, k})) / cell_dimensions[0]);
                if (i != grid_dimensions[0] - 1 && j != grid_dimensions[1] - 1)
                    Hz({i, j, k}) -= TimeStep * get_inv_permeability({i, j, k}) *
                                     ((Ey({i + 1, j, k}) - Ey({i, j, k})) / cell_dimensions[0] - (Ex({i, j + 1, k}) - Ex({i, j, k})) / cell_dimensions[1]);
            }
        }
    }
}
