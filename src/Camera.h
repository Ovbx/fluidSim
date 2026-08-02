#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>


class Camera {
public:
    Camera(glm::vec3 target, float distance, float yaw, float pitch);
    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yOffset);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    void updatePosition();
    //stadard vec3 kinda
    glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float m_distance;
    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoomSpeed;
    //projection matrix values
    float m_fov = 45.0f;
    float m_aspectRatio = 1280.0f / 1080.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 100.0f;
    //mat4 meaning 4x4 matrix
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

};
