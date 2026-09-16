#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Velocity2D {
  double u, v;
};
class StaggeredGrid {
public:
    void setBndU();
    void setBndV();
    void setBndPressure();
    void copyPreviousVelocities();
    StaggeredGrid(int nx, int ny, double dt, double gridSpacing);
    void addForces(int i, int j, double fx, double fy);
    void diffuseVelocity(double diff);
    void project();
    void advectVelocity();
    void injectDensity();
    void diffuseDensity();
    void advectDensity();
    void fluidSolver();
    std::vector<float> displaySolver(float worldSize, float minScale, float maxScale);
private:
    int m_nx, m_ny;
    double m_dx, m_dy, m_dz, m_dt;

    std::vector<double> m_density; //density aka Nx * Ny
    std::vector<double> m_pressure; //pressure
    std::vector<double> m_u; //x-vel
    std::vector<double> m_v; //y-vel Nx * (Ny + 1)

    //previous values are used for the jos stam backtracking (in time) advection implementation
    std::vector<double> m_dPrev;
    std::vector<double> m_pPrev;
    std::vector<double> m_uPrev;
    std::vector<double> m_vPrev;


    int m_sweepCount = 20;
    glm::vec2 cellToWorldPosition(int i, int j, float worldSize) const;
    double sampleU(int i, int j) const;
    double sampleV(int i, int j) const;
    float computeAngle(double u, double v) const;
    float computeMagnitude(double u, double v) const;
    std::vector<float> buildInstanceData(float worldSize, float minScale, float maxScale) const;

    //advection
    Velocity2D assembleVelocityAtU(int i, int j);
    Velocity2D assembleVelocityAtV(int i, int j);
    Velocity2D assembleVelocityAtDensity(int i, int j);
    double backtraceAndSampleDensity(Velocity2D velocity, glm::vec2 position);
    //helper
    //density and pressure although the same math is seperated for future me to know, +2 is to account for the ghost cells, returns count of array elements
    inline int densityCount(int nx, int ny) const {
        return (nx + 2) * (ny + 2);
    }
    inline int pressureCount(int nx, int ny) const {
        return (nx + 2) * (ny + 2);
    }
    inline int uCount(int nx, int ny) const {
        return (nx + 1) * (ny + 2);
    }
    inline int vCount(int nx, int ny) const {
        return (nx + 2) * (ny + 1);
    }
    //likewise, for future me to know
    inline int indexCenter(int i, int j) const {
        return (j + 1) * (m_nx + 2) + (i + 1);
    }
    inline int indexU(int i, int j) const  {
        return (j + 1) * (m_nx + 1) + i;
    }
    inline int indexV(int i, int j) const {
        return j * (m_nx + 2) + (i + 1);
    }

    inline glm::vec2 cellToPosition(int i, int j) {
      glm::vec2 position = {(i + 0.5) * m_dx, (j + 0.5) * m_dy};
      return position;
    }


};
