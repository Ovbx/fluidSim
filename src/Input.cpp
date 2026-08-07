#include "Input.h"
#include "Camera.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        std::cout << "clicked\n";
    }
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    // }
}
void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    //setup
    static float lastX = 640.0f, lastY = 540.0f;
    static bool firstMouse = true;

    //no last position yet so make last x, y as first xpos,ypos
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    //calc offsets
    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;

    //store latest
    lastX = xPos;
    lastY = yPos;
    //if not left click don't move camera
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)!= GLFW_PRESS) {
        return;
    }
    //pass to camera
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        cam->processMouseMovement(xOffset, yOffset);
    }
}
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        cam->processMouseScroll(yOffset);
    }
}