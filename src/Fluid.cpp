#include "Fluid.h"
#include <vector>

StaggeredGrid::StaggeredGrid(int nx, int ny, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_dx(gridSpacing), m_dy(gridSpacing), m_dt(dt), m_d(densityCount(nx, ny), 0.0), m_u(uCount(nx, ny), 0.0), m_v(vCount(nx, ny), 0.0), m_dPrev(densityCount(nx, ny), 0.0), m_uPrev(uCount(nx, ny), 0.0), m_vPrev(vCount(nx, ny), 0.0)   {
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