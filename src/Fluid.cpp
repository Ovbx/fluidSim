#include "Fluid.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

StaggeredGrid::StaggeredGrid(int nx, int ny, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_dx(gridSpacing), m_dy(gridSpacing), m_dt(dt), m_density(densityCount(nx, ny), 0.0), m_pressure(pressureCount(nx, ny), 0.0),m_u(uCount(nx, ny), 0.0), m_v(vCount(nx, ny), 0.0), m_dPrev(densityCount(nx, ny), 0.0), m_pPrev(pressureCount(nx, ny), 0.0), m_uPrev(uCount(nx, ny), 0.0), m_vPrev(vCount(nx, ny), 0.0)   {
    //hello world
}
void StaggeredGrid::setBndU () {
    //ghost rows (tangential no slip mirror negate)
    for (int i = 0; i <=m_nx; i++) {
        m_u.at(indexU(i, -1)) = -m_u.at(indexU(i, 0));
        m_u.at(indexU(i, m_ny)) = -m_u.at(indexU(i, m_ny - 1));
    }
    //wall faces (normal, zero)
    for(int j = 0; j <= m_ny - 1; j++) {
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
    for (int i = 0; i <= m_nx - 1; i++) {
        m_v.at(indexV(i, 0)) = 0.0;
        m_v.at(indexV(i, m_ny)) = 0.0;
    }
}
void StaggeredGrid::setBndPressure() {
  for (int i = 0; i <= m_ny; i++) {
    //ghost
    m_pressure.at(indexCenter(-1, i)) = m_pressure.at(indexCenter(0, i));
    m_pressure.at(indexCenter(m_nx, i)) = m_pressure.at(indexCenter(m_nx - 1, i));
  }
  for (int j = 0; j <= m_nx; j++) {
    m_pressure.at(indexCenter(j, -1)) = m_pressure.at(indexCenter(j, 0));
    m_pressure.at(indexCenter(j, m_ny))= m_pressure.at(indexCenter(j, m_ny - 1));
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
    //discretized rate of diffusion per grid cell
    double rateOfDiffusion = m_dt * diff / (m_dx * m_dy);
    double denominator = 1.0 + 4.0 * rateOfDiffusion;
    //similar to jos stam's implementation in "Real-Time Fluid Dynamics for Games"
    for (k = 0; k < m_sweepCount; k++) {
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
  int i, j, k;
  double h = 1.0 / m_nx;
  std::vector<double> div(pressureCount(m_nx, m_ny), 0.0);
  for (i = 0; i <= m_nx - 1; i++) {
    for (j = 0; j <= m_ny - 1; j++) {
      div.at(indexCenter(i, j)) = -0.5* h * (m_u.at(indexU(i+1, j)) - m_u.at(indexU(i, j)) + m_v.at(indexV(i, j+1)) - m_v.at(indexV(i, j)));

      m_pressure.at(indexCenter(i, j)) = 0.0;
    }
  }

  setBndPressure();
  for(k = 0; k <= m_sweepCount; k++) {
    for (i = 0; i <=m_nx - 1; i++) {
      for (j = 0; j <= m_ny - 1; j++) {
        m_pressure.at(indexCenter(i, j)) = (div.at(indexCenter(i, j)) + m_pressure.at(indexCenter(i-1, j)) + m_pressure.at(indexCenter(i+1, j)) + m_pressure.at(indexCenter(i, j - 1)) + m_pressure.at(indexCenter(i, j+1))) /4.0;
      }
    }
    setBndPressure();
  }

  for (i = 0; i <= m_nx - 1; i++) {
    for (j = 0; j<= m_ny - 1; j++) {
      m_u.at(indexU(i, j)) -= 0.5 * (m_pressure.at(indexCenter(i + 1, j)) - m_pressure.at(indexCenter(i-1, j))) / h;
      m_v.at(indexV(i, j)) -= 0.5 * (m_pressure.at(indexCenter(i, j+1))  - m_pressure.at(indexCenter(i, j-1))) /h;
    }
  }
setBndU();
setBndV();
}
void StaggeredGrid::advectVelocity() {

}
void StaggeredGrid::injectDensity() {

}
void StaggeredGrid::diffuseDensity() {

}
void StaggeredGrid::advectDensity() {

}

void StaggeredGrid::fluidSolver() {
  static bool forceApplied = false;
  if (!forceApplied) {
    addForces(2, 2, 25, 50);
    forceApplied = true;
  }
  copyPreviousVelocities();
  diffuseVelocity(0.0001);
  project();

  //advect velocity
  //project velocity

  //inject density
  //diffuse density
  //advect density
}

std::vector<float> StaggeredGrid::displaySolver(float worldSize, float minScale, float maxScale) {
  return buildInstanceData(worldSize, minScale, maxScale);
}
glm::vec2 StaggeredGrid::cellToWorldPosition(int i, int j, float worldSize) const {
  float distanceFromCenterToEdge = 0.5f;
  float worldX = (((i + distanceFromCenterToEdge) / m_nx ) - distanceFromCenterToEdge) * worldSize;
  float worldY = (((j + distanceFromCenterToEdge) / m_ny) - distanceFromCenterToEdge) * worldSize;
  glm::vec2 worldPosition = glm::vec2(worldX, worldY);
  return worldPosition;
}
double StaggeredGrid::sampleU(int i, int j) const {
  double uCenter = (m_u.at(indexU(i, j)) + m_u.at(indexU(i + 1, j))) / 2.0;
  return uCenter;
}
double StaggeredGrid::sampleV(int i, int j) const {
  double vCenter = (m_v.at(indexV(i, j)) + m_v.at(indexV(i, j +1))) / 2.0;
  return vCenter;
}
float StaggeredGrid::computeAngle(double u, double v) const {
  float angle = std::atan2(v, u);
  return angle;
}
float StaggeredGrid::computeMagnitude(double u, double v) const {
  float magnitude = std::sqrt(u*u + v*v);
  return magnitude;
}
std::vector<float> StaggeredGrid::buildInstanceData(float worldSize, float minScale, float maxScale) const {
  std::vector<float> instanceData;
  glm::vec2 worldPosition;
  float angle, magnitude, scale;
  double u, v;
  for (int i = 0; i < m_nx; i++) {
    for (int j = 0; j < m_ny; j++) {
      u = sampleU(i, j);
      v = sampleV(i, j);
      angle = computeAngle(u, v);
      magnitude = computeMagnitude(u, v);
      scale = std::clamp(static_cast<float>(magnitude), minScale, maxScale);
      worldPosition = cellToWorldPosition(i, j, worldSize);
      instanceData.push_back(worldPosition.x);
      instanceData.push_back(worldPosition.y);
      instanceData.push_back(angle);
      instanceData.push_back(scale);
    }
  }
  return instanceData;
}
