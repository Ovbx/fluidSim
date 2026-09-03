#include "Fluid.h"
#include <vector>

StaggeredGrid::StaggeredGrid(int nx, int ny, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_dx(gridSpacing), m_dy(gridSpacing), m_dt(dt), m_d(densityCount(nx, ny), 0.0), m_p(pressureCount(nx, ny), 0.0),m_u(uCount(nx, ny), 0.0), m_v(vCount(nx, ny), 0.0), m_dPrev(densityCount(nx, ny), 0.0), m_pPrev(pressureCount(nx, ny), 0.0), m_uPrev(uCount(nx, ny), 0.0), m_vPrev(vCount(nx, ny), 0.0)   {
    //hello world
}

void StaggeredGrid::addForces(int i, int j, double fx, double fy) {
    //m_u[] is a std::vector:operator[], it does pointer arithmatic to gets the address, dereferences it, and give the value stored as a reference. 
    m_u[indexU(i, j)] += m_dt * fx;
    m_v[indexV(i, j)] += m_dt * fy;
}
void StaggeredGrid::diffuseVelocity(std::vector<double>& u, std::vector<double>& uPrev, std::vector<double>& v,std::vector<double>& vPrev, double diff ) {

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