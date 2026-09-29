#include "Fluid3D.h"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

StaggeredGrid3D::StaggeredGrid3D(int nx, int ny, int nz, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_nz(nz), m_dx(gridSpacing), m_dy(gridSpacing), m_dz(gridSpacing), m_dt(dt), m_density(densityCount(nx, ny, nz), 0.0), m_pressure(pressureCount(nx, ny, nz), 0.0), m_u(uCount(nx, ny, nz), 0.0), m_v(vCount(nx, ny, nz), 0.0), m_w(wCount(nx, ny, nz), 0.0) {
    //hello world in 3d
}

void StaggeredGrid3D::setBndU() {
    
}

void StaggeredGrid3D::setBndV() {

}

void StaggeredGrid3D::setBndW() {

}

void StaggeredGrid3D::setBndPressure() {

}

void StaggeredGrid3D::setBndDensity() {

}

void StaggeredGrid3D::copyPreviousVelocities() {
    m_uPrev = m_u;
    m_vPrev = m_v;
    m_wPrev = m_w;
}

void StaggeredGrid3D::copyPreviousDensities() {
    m_densityPrev = m_density;
}
void StaggeredGrid3D::addForces(int i, int j, int k, double fx, double fy, double fz) {
    m_u[indexU(i, j, k)] = m_dt * fx;
    m_v[indexV(i, j, k)] = m_dt * fy;
    m_w[indexW(i, j, k)] = m_dt * fz;
}

void StaggeredGrid3D::diffuseVelocity(double diff) {
    int i, j, k;
    double rateOfDiffusion = m_dt * diff / (m_dx * m_dx);
    double denominator = 1.0 + 6.0 * rateOfDiffusion;
    for (int sweep = 0; sweep < m_sweepCount; sweep++) {
        for (k = 0; k <= m_nz -1; k++) {
            for (j = 0; j <= m_ny - 1; j++) {
                for (i  = 0; i <= m_nx - 1; i++) {
                    m_u[indexU(i, j, k)] = (m_uPrev[indexU(i, j, k)] + rateOfDiffusion * (m_u[indexU(i-1, j, k)] + m_u[indexU(i + 1, j, k)] + m_u[indexU(i, j-1, k)] + m_u[indexU(i, j+1, k)] + m_u[indexU(i, j, k-1)] + m_u[indexU(i, j, k+1)])) / denominator;
                }
            }
        }
        setBndU();
        for (k = 0; k <= m_nz - 1; k++) {
            for (j = 0; j <= m_ny -1; j++) {
                for (i = 0; i <= m_nx -1; i++) {
                    m_v[indexV(i, j, k)] = (m_vPrev[indexV(i, j, k)] + rateOfDiffusion * (m_v[indexV(i-1, j, k)] + m_v[indexV(i + 1, j, k)] + m_v[indexV(i, j-1, k)] + m_v[indexV(i, j+1, k)] + m_v[indexV(i, j, k-1)] + m_v[indexV(i, j, k+1)]))/ denominator;
                }
            }
        }
        setBndV();
        for (k = 0; k <= m_nz - 1; k++) {
            for (j =0 ; j <= m_ny -1; j++) {
                for (i = 0; i <= m_nx -1; i++) {
                    m_w[indexW(i, j, k)] = (m_wPrev[indexW(i, j, k)] + rateOfDiffusion * (m_w[indexW(i-1, j, k)] + m_w[indexW(i+1, j, k)] + m_w[indexW(i, j-1, k)] + m_w[indexW(i, j+1, k)] + m_w[indexW(i, j, k-1)] + m_w[indexW(i, j, k+1)])) / denominator;
                }
            }
        }
        setBndW();
    }
}

void StaggeredGrid3D::project () {

}

void StaggeredGrid3D::advectVelocity() {

}

void StaggeredGrid3D::addDensity() {

}

void StaggeredGrid3D::diffuseDensity() {

}

void StaggeredGrid3D::advectDensity() {

}

void StaggeredGrid3D::fluidSolver() {
    addForces(2, 2, 2, 50, 50 , 50);
    copyPreviousVelocities();
    //diffuse
    //project
    //add densities
    //diffuse
    //projet
}
//std::vector<float> StaggeredGrid3D::displaySolver(float worldSize, float minScale, float maxScale) {
//    
//}
//
