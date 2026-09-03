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
    int i, j, k;
    int sweepCounter = 20;
    //discretized rate of diffusion per grid cell
    double rateOfDiffusion = m_dt * diff / (m_dx * m_dy);
    //similar to jos stam's implementation in "Real-Time Fluid Dynamics for Games"
    for (k = 0; k < sweepCounter; k++) {
        for (i = 1; i <= m_nx - 1; i++) {
            for (j = 1; j <= m_ny; j++) {
                u[indexU(i, j)] = (uPrev[indexU(i, j)] + rateOfDiffusion * (u[indexU(i - 1, j)] + u[indexU(i + 1, j)] + u[indexU(i, j - 1)] + u[indexU(i, j + 1)])) / (1 + (4* rateOfDiffusion));
            }
        }
        for (i = 1; i <= m_nx; i++) {
            for (j = 1; j <= m_ny - 1; j++) {
                v[indexV(i, j)] = (vPrev[indexV(i, j)] +  rateOfDiffusion*(v[indexV(i - 1, j)] + v[indexV(i + 1, j)] + v[indexV(i, j - 1)] + v[indexV(i, j + 1)])) / (1 + (4 * rateOfDiffusion));
            }
        }
    }
    //set bnd not here yet
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
    //addVelocity();
    //diffuseVelocity();
    //project velocity
    //advect velocity
    //project velocity

    //inject density
    //diffuse density
    //advect density
}

void StaggeredGrid::displaySolver() {
    
}