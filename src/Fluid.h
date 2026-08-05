#pragma once
#include <vector>

class EulerianGrid {
    public:
    EulerianGrid();
    int width, height;
    void addForces();
    void diffuseVelocity();
    void project();
    void advectVelocity();
    void injectDensity();
    void advectDensity();
    void fluidSolver();
    void displaySolver();



    inline int mapping1D(int x, int y) const {
        return x + y * width;
    }

    private:
        std::vector<float> m_velocityX;
        std::vector<float> m_velocityY;
        std::vector<float> m_velocityX_prev;
        std::vector<float> m_velocityY_prev;
        std::vector<float> m_density;
        std::vector<float> m_density_prev;

};
