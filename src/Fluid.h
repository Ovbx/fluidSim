#pragma once
#include <vector>

class StaggeredGrid {
public:
    StaggeredGrid(int nx, int ny, double dt, double gridSpacing);
    void addForces();
    void diffuseVelocity();
    void project();
    void advectVelocity();
    void injectDensity();
    void advectDensity();
    void fluidSolver();
    void displaySolver();

private:
    int m_nx, m_ny;
    double m_dx, m_dy, m_dz, m_dt;

    std::vector<double> m_d; //density aka Nx * Ny
    std::vector<double> m_p; //pressure
    std::vector<double> m_u; //x-vel
    std::vector<double> m_v; //y-vel Nx * (Ny + 1)

    //previous values are used for the jos stam backtracking (in time) advection implementation
    std::vector<double> m_dPrev;
    std::vector<double> m_pPrev;
    std::vector<double> m_uPrev;
    std::vector<double> m_vPrev;
    


    //helper
    //density and pressure although the same math is seperated for future me to know
    inline int densityCount(int nx, int ny) const {
        return (nx + 2) * (ny + 2);
    }
    inline int pressureCount(int nx, int ny) const {
        return (nx + 2) * (ny + 2);
    }
    inline int uCount(int nx, int ny) const {
        return (nx + 1) * ny;
    }
    inline int vCount(int nx, int ny) const {
        return nx * (ny + 1);
    }
    //likewise, for future me to know 
    inline int indexDensity(int i, int j) const {
        return (j + 1) * (m_nx + 2) + (i + 1);
    }
    inline int indexPressure(int i, int j) const {
        return (j + 1) * (m_nx + 2) + (i + 1);
    }
    inline int indexU(int i, int j) const  {
        return (j + 1) * (m_nx + 1) + i;
    }
    inline int indexV(int i, int j) const {
        return j * (m_nx + 2) + (i + 1);
    }




};
