#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);