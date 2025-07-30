#include "common/types.hpp"
#include "yee/grid.hpp"

void YeeGrid::naive_grid_update(real time_step) {
    // Amperes law
    for (int k = 0; k < grid_dimensions[2]; k++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {
            for (int i = 0; i < grid_dimensions[0]; i++) {
                if (j != 0 && k != 0) {
                    ex({ i, j, k }) += time_step * get_inv_permittivity({ i, j, k }) *
                                       ((hz({ i, j, k }) - hz({ i, j, k - 1 })) / cell_dimensions[1] -
                                        (hy({ i, j, k }) - hy({ i, j - 1, k })) / cell_dimensions[2]);
                }
                if (i != 0 && k != 0) {
                    ey({ i, j, k }) += time_step * get_inv_permittivity({ i, j, k }) *
                                       ((hx({ i - 1, j, k }) - hx({ i, j, k })) / cell_dimensions[2] -
                                        (hz({ i, j, k - 1 }) - hz({ i, j, k })) / cell_dimensions[0]);
                }
                if (i != 0 && j != 0) {
                    ez({ i, j, k }) += time_step * get_inv_permittivity({ i, j, k }) *
                                       ((hy({ i, j - 1, k }) - hy({ i, j, k })) / cell_dimensions[0] -
                                        (hx({ i - 1, j, k }) - hx({ i, j, k })) / cell_dimensions[1]);
                }
            }
        }
    }
    // Faraday's law
    for (int k = 0; k < grid_dimensions[2]; k++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {
            for (int i = 0; i < grid_dimensions[0]; i++) {
                if (j != grid_dimensions[1] - 1 && k != grid_dimensions[2] - 1) {
                    hx({ i, j, k }) -= time_step * get_inv_permeability({ i, j, k }) *
                                       ((ez({ i, j + 1, k }) - ez({ i, j, k })) / cell_dimensions[1] -
                                        (ey({ i, j, k + 1 }) - ey({ i, j, k })) / cell_dimensions[2]);
                }
                if (i != grid_dimensions[0] - 1 && k != grid_dimensions[2] - 1) {
                    hy({ i, j, k }) -= time_step * get_inv_permeability({ i, j, k }) *
                                       ((ex({ i, j, k + 1 }) - ex({ i, j, k })) / cell_dimensions[2] -
                                        (ez({ i + 1, j, k }) - ez({ i, j, k })) / cell_dimensions[0]);
                }
                if (i != grid_dimensions[0] - 1 && j != grid_dimensions[1] - 1) {
                    hz({ i, j, k }) -= time_step * get_inv_permeability({ i, j, k }) *
                                       ((ey({ i + 1, j, k }) - ey({ i, j, k })) / cell_dimensions[0] -
                                        (ex({ i, j + 1, k }) - ex({ i, j, k })) / cell_dimensions[1]);
                }
            }
        }
    }
}
