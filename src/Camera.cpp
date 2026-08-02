#include "Camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera(glm::vec3 target, float distance, float yaw, float pitch) 
: m_target(target), m_distance(distance), m_yaw(yaw), m_pitch(pitch) {
    updatePosition();
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch) {
    xOffset *= SENSITIVITY;
    yOffset *= SENSITIVITY;

    m_yaw += xOffset;
    m_pitch += yOffset;
    
    if (constraintPitch) {
        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        }
        if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }
    }

    updatePosition();
}

void Camera::processMouseScroll(float yOffset) {
    m_distance -= yOffset;
    if (m_distance < 1.0f) {
        m_distance = 1.0f;
    }
    if (m_distance > 45.0f) {
        m_distance = 45.0f;
    }
    updatePosition();
}

glm::mat4 Camera::getViewMatrix() {
    return m_viewMatrix;
}
glm::mat4 Camera::getProjectionMatrix() {
    return m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}
void Camera::updatePosition() {
    //spherical to cartesian coordinates
    float x = m_distance*cos(glm::radians(m_pitch))*cos(glm::radians(m_yaw));
    float y = m_distance*sin(glm::radians(m_pitch));
    float z = m_distance*sin(glm::radians(m_yaw))*cos(glm::radians(m_pitch));

    glm::vec3 position = m_target + glm::vec3(x, y, z); 
    m_viewMatrix = glm::lookAt(position, m_target, m_up);
}