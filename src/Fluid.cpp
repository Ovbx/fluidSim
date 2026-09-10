#include "Fluid.h"
#include <vector>

StaggeredGrid::StaggeredGrid(int nx, int ny, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_dx(gridSpacing), m_dy(gridSpacing), m_dt(dt), m_d(densityCount(nx, ny), 0.0), m_p(pressureCount(nx, ny), 0.0),m_u(uCount(nx, ny), 0.0), m_v(vCount(nx, ny), 0.0), m_dPrev(densityCount(nx, ny), 0.0), m_pPrev(pressureCount(nx, ny), 0.0), m_uPrev(uCount(nx, ny), 0.0), m_vPrev(vCount(nx, ny), 0.0)   {
    //hello world
}
void StaggeredGrid::setBndU () {
    //ghost rows (tangential no slip mirror negate)
    for (int i = 0; i <=m_nx; i++) {
        m_u.at(indexU(i, -1)) = -m_u.at(indexU(i, 0));
        m_u.at(indexU(i, m_ny)) = -m_u.at(indexU(i, m_ny - 1));
    }
    //wall faces (normal, zero)
    for(int j = 1; j <= m_ny; j++) {
        m_u.at(indexU(0, j)) = 0.0;
        m_u.at(indexU(m_nx, j)) = 0.0;
    }
}

void StaggeredGrid::setBndV () {
    for (int k = 0; k <= m_ny; k++) {
        //ghost face
        m_v.at(indexV(-1, k)) = -m_v.at(indexV(0, k));
        m_v.at(indexV(m_nx, k)) = -m_v.at(indexV(m_nx - 1, k));
    }
        //wall face
    for (int j = 1; j <= m_nx - 1; j++) {
        m_v.at(indexV(j, 0)) = 0;
        m_v.at(indexV(j, m_ny)) = 0;
    }
}

void StaggeredGrid::copyPreviousVelocities() {
    m_uPrev = m_u;
    m_vPrev = m_v;
}


void StaggeredGrid::addForces(int i, int j, double fx, double fy) {
    //m_u[] is a std::vector:operator[], it does pointer arithmatic to gets the address, dereferences it, and give the value stored as a reference.
    m_u.at(indexU(i, j)) += m_dt * fx;
    m_v.at(indexV(i, j)) += m_dt * fy;
}

void StaggeredGrid::diffuseVelocity(double diff) {
    int i, j, k;
    int sweepCounter = 20;
    //discretized rate of diffusion per grid cell
    double rateOfDiffusion = m_dt * diff / (m_dx * m_dy);
    double denominator = 1.0 + 4.0 * rateOfDiffusion;
    //similar to jos stam's implementation in "Real-Time Fluid Dynamics for Games"
    for (k = 0; k < sweepCounter; k++) {
        for (i = 1; i <= m_nx - 1; i++) {
            for (j = 0; j <= m_ny - 1; j++) {
                //gauss-seidel relaxation
                m_u.at(indexU(i, j)) = (m_uPrev.at(indexU(i, j)) + rateOfDiffusion * ( m_u.at(indexU(i - 1, j)) + m_u.at(indexU(i + 1, j)) + m_u.at(indexU(i, j - 1)) + m_u.at(indexU(i, j + 1)))) / denominator;

            }
        }
        setBndU();
        for (i = 0; i <= m_nx - 1; i++) {
            for (j = 1; j <= m_ny - 1; j++) {
                m_v.at(indexV(i, j)) = (m_vPrev.at(indexV(i, j)) +  rateOfDiffusion*(m_v.at(indexV(i - 1, j)) + m_v.at(indexV(i + 1, j)) + m_v.at(indexV(i, j - 1)) + m_v.at(indexV(i, j + 1)))) / denominator;

            }
        }
        setBndV();
    }
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
    addForces(2, 2, 50, 50);
    copyPreviousVelocities();
    diffuseVelocity(0.0001);
    //project velocity
    //advect velocity
    //project velocity

    //inject density
    //diffuse density
    //advect density
}

void StaggeredGrid::displaySolver() {
  
}
