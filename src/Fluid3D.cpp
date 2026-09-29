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

}

void StaggeredGrid3D::copyPreviousDensities() {

}


