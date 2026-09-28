#pragma once

#include <glm/glm.h>
#include <vector>

struct Velocity3D {
    double u, v, w;
}
class StaggeredGrid3D {
public:
    void setBndU();
    void setBndV();
    void setBndW();
    void setBndPressure();
    void setBndDensity();
    void copyPreviousVelocities();
    void copyPreviousDensities();
    StaggeredGrid3D(int nx, int ny, int nz, double dt, double gridSpacing);
    void addForces();
    void diffuseVelocity();
    void project();
    void advectVelocity();
    void addDensity();
    void diffuseDensity();
    void advectDensity();
    void fluidSolver();
    
private:
    int m_nx, m_ny, m_nz;
    double m_dx, m_dy, m_dz, m_dt;

    std::vector<double> m_density;
    std::vector<double> m_pressure;
    std::vector<double> m_u;
    std::vector<double> m_v;
    std::vector<double> m_w;

    std::vector<double> m_densityPrev;
    std::vector<double> m_pressurePrev;
    std::vector<double> m_uPrev;
    std::vector<double> m_vPrev;
    std::vector<double> m_wPrev;
    
    inline int densityCount(int nx, int ny, int nz) const {
        return (nx + 2) * (ny + 2) * (nz + 2);
    }

    inline int pressureCount(int nx, int ny, int nz) const {
        return (nx + 2) * (ny + 2) * (nz + 2);
    }

    inline int uCount(int nx, int ny, int nz) const {
        return (nx + 1) * (ny + 2) * (nz + 2);
    }

    inline int vCount(int nx, int ny, int nz) const {
        return (nx + 2) * (ny + 1) * (nz + 2);
    }

    inline int wCount(int nx, int ny, int nz) const {
        return (nx + 2) * (ny + 2) * (nz + 1);
    }
    inline int indexCenter(int i, int j, int k) const {
        return (k+1)*(m_nx+2)*(m_ny+2) + (j+1)*(m_nx + 2) + (i+1);
    }

    inline int indexU() const {
        
    }

    inline int indexV() const {

    }

    inline int indexW() const {

    }

    inline glm::vec2 cellToPosition () {

    }

    inline glm::vec2 cellToUPosition() {

    }

    inline glm::vec2 celltoVPosition () {

    }

    inline glm::vec2 cellToWPosition () {

    }


    
}
