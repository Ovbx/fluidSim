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
    std::vector<double> m_u; //x-vel (Nx + 1) * Ny
    std::vector<double> m_v; //y-vel Nx * (Ny + 1)

    std::vector<double> m_dPrev;
    std::vector<double> m_uPrev;
    std::vector<double> m_vPrev;
    
    //helper array for inside proect step
    std::vector<double> m_p;

    //helper for density
    inline double calculateDensity(int nx, int ny) const {
        nx * ny;
    }
    inline double calculateXVelocity(int nx, int ny) const {
        (nx + 1) * ny;
    }
    inline double calculateYVelocity(int nx, int ny) const {
        nx * (ny + 1);
    }





};
