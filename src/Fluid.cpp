#include "Fluid.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>

StaggeredGrid::StaggeredGrid(int nx, int ny, double dt, double gridSpacing) : m_nx(nx), m_ny(ny), m_dx(gridSpacing), m_dy(gridSpacing), m_dt(dt), m_density(densityCount(nx, ny), 0.0), m_pressure(pressureCount(nx, ny), 0.0),m_u(uCount(nx, ny), 0.0), m_v(vCount(nx, ny), 0.0), m_densityPrev(densityCount(nx, ny), 0.0), m_pressurePrev(pressureCount(nx, ny), 0.0), m_uPrev(uCount(nx, ny), 0.0), m_vPrev(vCount(nx, ny), 0.0)   {
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
void StaggeredGrid::setBndDensity() {
  for (int i = 0; i <= m_ny; i++) {
    m_density[indexCenter(-1, i)] = m_density[indexCenter(0,i)];
    m_density[indexCenter(m_nx, i)] = m_density[indexCenter(m_nx - 1, i)];
  }
  for (int j = 0; j <= m_nx; j++) {
    m_density[indexCenter(j, -1)] = m_density[indexCenter(j, 0)];
    m_density[indexCenter(j, m_ny)] = m_density[indexCenter(j, m_ny -1)];
  }
}
void StaggeredGrid::copyPreviousVelocities() {
    m_uPrev = m_u;
    m_vPrev = m_v;
}
void StaggeredGrid::copyPreviousDensities() {
  m_densityPrev = m_density;
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
        for (i = 0; i <= m_nx - 1; i++) {
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
  int i, j;
  glm::vec2 position;
  Velocity2D velocity;
  //u
  for (i = 0; i <= m_nx - 1;i++) {
    for (j = 0; j <= m_ny - 1; j++) {
      position = cellToUPosition(i, j);
      velocity = assembleVelocityAtU(i, j);
      m_u[indexU(i, j)] = backtraceAndSampleU(velocity, position);

    }
  }
  //v
  for (i = 0; i <= m_nx - 1; i++) {
    for (j = 0; j <= m_ny - 1; j++) {
      position = cellToVPosition(i, j);
      velocity = assembleVelocityAtV(i, j);
      m_v[indexV(i, j)] = backtraceAndSampleV(velocity, position);
    }
  }
  setBndU();
  setBndV();
}

void StaggeredGrid::addDensity(int i, int j, double source) {
  m_density[indexCenter(i, j)] += m_dt * source;
}
void StaggeredGrid::diffuseDensity(double diff) {
  int i, j, k;
  double rateOfDiffusion = m_dt * diff / (m_dx * m_dy);
  double denominator = 1.0 + 4.0 * rateOfDiffusion;
  for (k = 0; k <= m_sweepCount; k++) {
    for (i = 0; i <= m_nx - 1; i++) {
      for (j = 0; j <= m_ny - 1; j++) {
        m_density[indexCenter(i, j)] = (m_densityPrev[indexCenter(i, j)] + rateOfDiffusion * (m_density[indexCenter(i-1, j)] + m_density[indexCenter(i + 1, j)] + m_density[indexCenter(i, j-1)] + m_density[indexCenter(i, j + 1)])) / denominator;
      }
    }
    setBndDensity();
  }
}

void StaggeredGrid::advectDensity() {
  int i, j;
  glm::vec2 position;
  Velocity2D velocity;
  for (i = 1; i <= m_nx - 2; i++) {
    for(j = 1; j <= m_ny - 2; j++) {
      position = cellToPosition(i, j);
      velocity = assembleVelocityAtDensity(i, j);
      m_density[indexCenter(i, j)] = backtraceAndSampleDensity(velocity, position);
    }
  }
  setBndDensity();
}

void StaggeredGrid::fluidSolver() {
  static bool forceApplied = false;
  if (!forceApplied) {
    addForces(8, 10, 100, 50);
    addForces(10, 25, 50, 30);
    forceApplied = false;
  }
  copyPreviousVelocities();
  diffuseVelocity(0.3);
  project();
  advectVelocity();
  project();
  addDensity(16, 30, 10);
  copyPreviousDensities();
  diffuseDensity(0.3);
  advectDensity();
}

//i, j to x, y
std::vector<float> StaggeredGrid::displaySolver(float worldSize, float minScale, float maxScale) {
  float smoothFactor = 0.1f;
  float maxMagnitudeThisFrame = findMaxMagnitude();
  float m_smoothedMaxMagnitude = m_smoothedMaxMagnitude + smoothFactor * (maxMagnitudeThisFrame - m_smoothedMaxMagnitude);
  return buildInstanceData(worldSize, minScale, maxScale, m_smoothedMaxMagnitude);
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

float StaggeredGrid::findMaxMagnitude() const {
  int i, j;
  double u, v;
  float magnitude;
  float largestMagnitude = 0.0f;
  for (i = 0; i < m_nx; i++) {
    for (j = 0; j < m_ny; j++) {
      u = sampleU(i, j);
      v = sampleV(i, j);
      magnitude = computeMagnitude(u, v);
      if (magnitude > largestMagnitude) {
        largestMagnitude = magnitude;
      }
    }
  }
  return largestMagnitude;
}

std::vector<float> StaggeredGrid::buildInstanceData(float worldSize, float minScale, float maxScale, float maxMagnitudeThisFrame) const {
  std::vector<float> instanceData;
  glm::vec2 worldPosition;
  float angle, magnitude, normalizedMagnitude, scale;
  double u, v;
  for (int i = 0; i < m_nx; i++) {
    for (int j = 0; j < m_ny; j++) {
      u = sampleU(i, j);
      v = sampleV(i, j);
      angle = computeAngle(u, v);
      magnitude = computeMagnitude(u, v);

      //color instanceData
      if (maxMagnitudeThisFrame > 0.0f) {
        normalizedMagnitude = glm::clamp(magnitude / maxMagnitudeThisFrame, 0.0f, 1.0f);
        normalizedMagnitude = pow(normalizedMagnitude, 0.5f);
      }
      else {
        normalizedMagnitude = 0.0f;
      }
      //the rest of the instanceData
      scale = glm::clamp(static_cast<float>(magnitude), minScale, maxScale);
      worldPosition = cellToWorldPosition(i, j, worldSize);
      instanceData.push_back(worldPosition.x);
      instanceData.push_back(worldPosition.y);
      instanceData.push_back(angle);
      instanceData.push_back(scale);
      instanceData.push_back(normalizedMagnitude);
    }
  }
  return instanceData;
}

//advection helpers
Velocity2D StaggeredGrid::assembleVelocityAtU(int i, int j){
  double uValue = m_u[indexU(i, j)];
  double vValue = (m_v[indexV(i-1, j)] + m_v[indexV(i, j)] + m_v[indexV(i-1, j+1)] + m_v[indexV(i, j+1)]) / 4.0;
  Velocity2D values = {uValue, vValue};
  return values;
}

Velocity2D StaggeredGrid::assembleVelocityAtV(int i, int j) {
  double uValue = (m_u[indexU(i, j-1)] + m_u[indexU(i + 1, j-1)] + m_u[indexU(i, j)] + m_u[indexU(i+1, j)]) / 4.0;
  double vValue = m_v[indexV(i, j)];
  Velocity2D values = {uValue, vValue};
  return values;
}

Velocity2D StaggeredGrid::assembleVelocityAtDensity(int i, int j) {
   double uValue = (m_u[indexCenter(i+1, j)] + m_u[indexCenter(i, j)]) / 2.0;
   double vValue = (m_v[indexCenter(i, j)] +  m_v[indexCenter(i, j+1)]) / 2.0;
   Velocity2D velocityValues = {uValue, vValue};
   return velocityValues;
}

double StaggeredGrid::backtraceAndSampleU(Velocity2D velocity, glm::vec2 position){
  float minX = 0.0f;
  float maxX = m_nx * m_dx;
  float minY = 0.5f * m_dy;
  float maxY = (m_ny - 0.5f) * m_dy;
  float x = position.x - m_dt * velocity.u;
  float y = position.y - m_dt * velocity.v;
  float clampedX = glm::clamp(x, minX, maxX);
  float clampedY = glm::clamp(y, minY, maxY);
  float gridCoordX = clampedX / m_dx;
  float gridCoordY = (clampedY / m_dy) - 0.5f;
  int i0 = glm::floor(gridCoordX);
  int j0 = glm::floor(gridCoordY);
  int i1 = i0 + 1;
  int j1 = j0 + 1;
  float s1 = gridCoordX - i0;
  float s0 = 1.0f - s1;
  float t1 = gridCoordY - j0;
  float t0 = 1.0f - t1;
  return s0 * (t0 * m_uPrev[indexU(i0, j0)] + t1 * m_uPrev[indexU(i0, j1)]) + s1 * (t0 * m_uPrev[indexU(i1, j0)] + t1 * m_uPrev[indexU(i1, j1)]);
}

double StaggeredGrid::backtraceAndSampleV(Velocity2D velocity, glm::vec2 position) {
  float minX = 0.5f * m_dx;
  float maxX = (m_nx - 0.5f) * m_dx;
  float minY = 0.0f;
  float maxY = m_ny * m_dy;
  float x = position.x - m_dt * velocity.u;
  float y = position.y - m_dt * velocity.v;
  float clampedX = glm::clamp(x, minX, maxX);
  float clampedY = glm::clamp(y, minY, maxY);
  float gridCoordX = (clampedX / m_dx) - 0.5f;
  float gridCoordY = clampedY / m_dy;
  int i0 = glm::floor(gridCoordX);
  int j0 = glm::floor(gridCoordY);
  int i1 = i0 + 1;
  int j1 = j0 + 1;
  float s1 = gridCoordX - i0;
  float s0 = 1.0f - s1;
  float t1 = gridCoordY - j0;
  float t0 = 1.0f - t1;
  return s0 * (t0 * m_vPrev[indexV(i0, j0)] + t1 * m_vPrev[indexV(i0, j1)]) + s1 * (t0 * m_vPrev[indexV(i1, j0)] + t1 * m_vPrev[indexV(i1, j1)]);
}

//pass in velocityatdensity
double StaggeredGrid::backtraceAndSampleDensity(Velocity2D velocity, glm::vec2 position){
  float minX = 0.5f * m_dx;
  float maxX =  (m_nx - 0.5f) * m_dx;
  float minY = 0.5f * m_dy;
  float maxY = (m_ny - 0.5f) * m_dy;
  float x = position.x - m_dt * velocity.u;
  float y = position.y - m_dt * velocity.v;
  float clampedX = glm::clamp(x, minX, maxX);
  float clampedY = glm::clamp(y, minY, maxY);
  float gridCoordX = (clampedX / m_dx) - 0.5f;
  float gridCoordY = (clampedY / m_dy) - 0.5f;
  int i0 = glm::floor(gridCoordX);
  int j0 = glm::floor(gridCoordY);
  int i1 = i0 + 1;
  int j1 = j0 + 1;

  float s1 = gridCoordX - i0;
  float s0 = 1.0f - s1;
  float t1 = gridCoordY - j0;
  float t0 = 1.0f - t1;
  return s0 * (t0 * m_densityPrev[indexCenter(i0, j0)] + t1 * m_densityPrev[indexCenter(i0, j1)]) + s1 * ( t0 * m_densityPrev[indexCenter(i1, j0)] + t1 * m_densityPrev[indexCenter(i1, j1)]);
}
