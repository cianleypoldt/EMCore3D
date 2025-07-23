// yeeGrid.cpp  (complete update function)
#include "yeeGrid.h"
#include <immintrin.h>

void YeeGrid::update(uint count, R _dt) {
    for (int i = 0; i < count; ++i) {
        const double dt = _dt;
        const index3& N = m_grid_dimensions;
        const int Nx = N[0], Ny = N[1], Nz = N[2];
        const int sy = Nx;
        const int sz = Nx * Ny;

        // ------------------------------------------------------------------
        //  helper: aligned load/store
        // ------------------------------------------------------------------
        auto load = [](const double* p) { return _mm256_load_pd(p); };
        auto store = [](double* p, __m256d v) { _mm256_store_pd(p, v); };

// ------------------------------------------------------------------
//  H-field (Faraday)
// ------------------------------------------------------------------
#pragma omp parallel for collapse(2) schedule(static)
        for (int k = 0; k < Nz - 1; ++k) {
            for (int j = 0; j < Ny - 1; ++j) {
                const int base = k * sz + j * sy;

                // ---------- vector chunk -------------
                const int i4 = (Nx - 1) & ~3; // largest multiple of 4 <= Nx-1
                for (int i = 0; i < i4; i += 4) {
                    const int idx = base + i;
                    const int idxX1 = idx + 1;
                    const int idxY1 = idx + sy;
                    const int idxZ1 = idx + sz;

                    __m256d ex = load(&m_Ex[idx]);
                    __m256d ex_y = load(&m_Ex[idxY1]);
                    __m256d ex_z = load(&m_Ex[idxZ1]);

                    __m256d ey = load(&m_Ey[idx]);
                    __m256d ey_x = load(&m_Ey[idxX1]);
                    __m256d ey_z = load(&m_Ey[idxZ1]);

                    __m256d ez = load(&m_Ez[idx]);
                    __m256d ez_x = load(&m_Ez[idxX1]);
                    __m256d ez_y = load(&m_Ez[idxY1]);

                    __m256d dEz_dy = _mm256_sub_pd(ez_y, ez);
                    __m256d dEy_dz = _mm256_sub_pd(ey_z, ey);
                    __m256d dEx_dz = _mm256_sub_pd(ex_z, ex);
                    __m256d dEz_dx = _mm256_sub_pd(ez_x, ez);
                    __m256d dEy_dx = _mm256_sub_pd(ey_x, ey);
                    __m256d dEx_dy = _mm256_sub_pd(ex_y, ex);

                    __m256d inv_mu = _mm256_div_pd(_mm256_set1_pd(dt),
                                                   load(&m_permeability[idx]));

                    __m256d hx = load(&m_Hx[idx]);
                    __m256d hy = load(&m_Hy[idx]);
                    __m256d hz = load(&m_Hz[idx]);

                    hx = _mm256_fmadd_pd(inv_mu, _mm256_sub_pd(dEz_dy, dEy_dz), hx);
                    hy = _mm256_fmadd_pd(inv_mu, _mm256_sub_pd(dEx_dz, dEz_dx), hy);
                    hz = _mm256_fmadd_pd(inv_mu, _mm256_sub_pd(dEy_dx, dEx_dy), hz);

                    store(&m_Hx[idx], hx);
                    store(&m_Hy[idx], hy);
                    store(&m_Hz[idx], hz);
                }
                // ---------- scalar tail -------------
                for (int i = i4; i < Nx - 1; ++i) {
                    const int idx = base + i;
                    const double dEz_dy = m_Ez[idx + sy] - m_Ez[idx];
                    const double dEy_dz = m_Ey[idx + sz] - m_Ey[idx];
                    const double dEx_dz = m_Ex[idx + sz] - m_Ex[idx];
                    const double dEz_dx = m_Ez[idx + 1] - m_Ez[idx];
                    const double dEy_dx = m_Ey[idx + 1] - m_Ey[idx];
                    const double dEx_dy = m_Ex[idx + sy] - m_Ex[idx];

                    const double inv_mu = dt / m_permeability[idx];

                    m_Hx[idx] += inv_mu * (dEz_dy - dEy_dz);
                    m_Hy[idx] += inv_mu * (dEx_dz - dEz_dx);
                    m_Hz[idx] += inv_mu * (dEy_dx - dEx_dy);
                }
            }
        }

        // ------------------------------------------------------------------
        //  E-field (Ampère) – same pattern: vector + scalar tail
        // ------------------------------------------------------------------
        /*
        #pragma omp parallel for collapse(2) schedule(static)
                for (int k = 1; k < Nz; ++k) {
                    for (int j = 1; j < Ny; ++j) {
                        const int base = k * sz + j * sy;

                        const int i4 = Nx & ~3;
                        for (int i = 1; i < i4; i += 4) {
                            const int idx = base + i;
                            const int idxM1 = idx - 1;
                            const int idxMY = idx - sy;
                            const int idxMZ = idx - sz;

                            __m256d hx = load(&m_Hx[idx]);
                            __m256d hxMY = load(&m_Hx[idxMY]);
                            __m256d hxMZ = load(&m_Hx[idxMZ]);

                            __m256d hy = load(&m_Hy[idx]);
                            __m256d hyM1 = load(&m_Hy[idxM1]);
                            __m256d hyMZ = load(&m_Hy[idxMZ]);

                            __m256d hz = load(&m_Hz[idx]);
                            __m256d hzM1 = load(&m_Hz[idxM1]);
                            __m256d hzMY = load(&m_Hz[idxMY]);

                            __m256d dHz_dy = _mm256_sub_pd(hz, hzMY);
                            __m256d dHy_dz = _mm256_sub_pd(hy, hyMZ);
                            __m256d dHx_dz = _mm256_sub_pd(hx, hxMZ);
                            __m256d dHz_dx = _mm256_sub_pd(hz, hzM1);
                            __m256d dHy_dx = _mm256_sub_pd(hy, hyM1);
                            __m256d dHx_dy = _mm256_sub_pd(hx, hxMY);

                            __m256d inv_eps = _mm256_div_pd(_mm256_set1_pd(dt),
                                                            load(&m_permittivity[idx]));

                            __m256d ex = load(&m_Ex[idx]);
                            __m256d ey = load(&m_Ey[idx]);
                            __m256d ez = load(&m_Ez[idx]);

                            ex = _mm256_fmadd_pd(inv_eps, _mm256_sub_pd(dHz_dy, dHy_dz), ex);
                            ey = _mm256_fmadd_pd(inv_eps, _mm256_sub_pd(dHx_dz, dHz_dx), ey);
                            ez = _mm256_fmadd_pd(inv_eps, _mm256_sub_pd(dHy_dx, dHx_dy), ez);

                            store(&m_Ex[idx], ex);
                            store(&m_Ey[idx], ey);
                            store(&m_Ez[idx], ez);
                        }
                        for (int i = i4; i < Nx; ++i) {
                            const int idx = base + i;
                            const double dHz_dy = m_Hz[idx] - m_Hz[idx - sy];
                            const double dHy_dz = m_Hy[idx] - m_Hy[idx - sz];
                            const double dHx_dz = m_Hx[idx] - m_Hx[idx - sz];
                            const double dHz_dx = m_Hz[idx] - m_Hz[idx - 1];
                            const double dHy_dx = m_Hy[idx] - m_Hy[idx - 1];
                            const double dHx_dy = m_Hx[idx] - m_Hx[idx - sy];

                            const double inv_eps = dt / m_permittivity[idx];

                            m_Ex[idx] += inv_eps * (dHz_dy - dHy_dz);
                            m_Ey[idx] += inv_eps * (dHx_dz - dHz_dx);
                            m_Ez[idx] += inv_eps * (dHy_dx - dHx_dy);
                        }
                    }
                    }*/
    }
}
