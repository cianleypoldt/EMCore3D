#include "../constants.h"
#include "yeeGrid.h"
#include <immintrin.h>

void YeeGrid::updateOMP(const uint count, const R time_step) {
    for (int step = 0; step < count; ++step) {
        const index3& N = m_grid_dimensions; // aliases for readability
        const int Nx = N[0], Ny = N[1], Nz = N[2];
        const int stride_y = Nx;
        const int stride_z = Nx * Ny;

        const R dt = time_step;

// -------------------------------------------------------------------------
//  Faraday:  H += dt/μ * curl E
// -------------------------------------------------------------------------
#pragma omp parallel for collapse(2) schedule(static)
        for (int k = 0; k < Nz - 1; ++k) {
            for (int j = 0; j < Ny - 1; ++j) {
                const int base = k * stride_z + j * stride_y;

#pragma omp simd
                for (int i = 0; i < Nx; ++i) {
                    const int idx = base + i;
                    const int idxX1 = idx + 1;        // i+1
                    const int idxY1 = idx + stride_y; // j+1
                    const int idxZ1 = idx + stride_z; // k+1

                    const R dEz_dy = m_Ez[idxY1] - m_Ez[idx];
                    const R dEy_dz = m_Ey[idxZ1] - m_Ey[idx];
                    const R dEx_dz = m_Ex[idxZ1] - m_Ex[idx];
                    const R dEz_dx = m_Ez[idxX1] - m_Ez[idx];
                    const R dEy_dx = m_Ey[idxX1] - m_Ey[idx];
                    const R dEx_dy = m_Ex[idxY1] - m_Ex[idx];

                    const R inv_mu = dt / m_permeability[idx];

                    m_Hx[idx] += inv_mu * (dEz_dy - dEy_dz);
                    m_Hy[idx] += inv_mu * (dEx_dz - dEz_dx);
                    m_Hz[idx] += inv_mu * (dEy_dx - dEx_dy);
                }
            }
        }

// -------------------------------------------------------------------------
//  Ampère:  E += dt/ε * curl H
// -------------------------------------------------------------------------
#pragma omp parallel for collapse(2) schedule(static)
        for (int k = 1; k < Nz; ++k) {
            for (int j = 1; j < Ny; ++j) {
                const int base = k * stride_z + j * stride_y;

#pragma omp simd
                for (int i = 1; i < Nx; ++i) {
                    const int idx = base + i;
                    const int idxM1 = idx - 1;        // i-1
                    const int idxMY = idx - stride_y; // j-1
                    const int idxMZ = idx - stride_z; // k-1

                    const R dHz_dy = m_Hz[idx] - m_Hz[idxMY];
                    const R dHy_dz = m_Hy[idx] - m_Hy[idxMZ];
                    const R dHx_dz = m_Hx[idx] - m_Hx[idxMZ];
                    const R dHz_dx = m_Hz[idx] - m_Hz[idxM1];
                    const R dHy_dx = m_Hy[idx] - m_Hy[idxM1];
                    const R dHx_dy = m_Hx[idx] - m_Hx[idxMY];

                    const R inv_eps = dt / m_permittivity[idx];

                    m_Ex[idx] += inv_eps * (dHz_dy - dHy_dz);
                    m_Ey[idx] += inv_eps * (dHx_dz - dHz_dx);
                    m_Ez[idx] += inv_eps * (dHy_dx - dHx_dy);
                }
            }
        }
    }
}
