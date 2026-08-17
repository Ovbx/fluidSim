#include "Fluid.h"
#include <vector>

StaggeredGrid::StaggeredGrid(int nx, int ny, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_dt(dt), m_dx(gridSpacing), m_dy(gridSpacing), m_d(calculateDensity(nx, ny), 0.0f), m_dPrev(calculateDensity(nx, ny), 0.0f), m_u(calculateXVelocity(nx, ny), 0.0f), m_uPrev(calculateXVelocity(nx, ny), 0.0f), m_v(calculateYVelocity(nx, ny), 0.0f), m_vPrev(calculateYVelocity(nx, ny), 0.0f)   {
    //hello world
}

void StaggeredGrid::addForces() {
    
}
void StaggeredGrid::diffuseVelocity() {

}
void StaggeredGrid::project() {

}
void StaggeredGrid::advectVelocity() {

}
void StaggeredGrid::injectDensity() {

}
void StaggeredGrid::advectDensity() {

}

void StaggeredGrid::fluidSolver() {

}

void StaggeredGrid::displaySolver() {
    
}