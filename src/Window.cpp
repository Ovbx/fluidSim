#include "Window.h"
#include <glad/glad.h>
#include <iostream>
Window::Window(int width, int height, const char* title) : m_width(width), m_height(height) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(
        width,
        height,
        title,
        NULL,
        NULL
    );
    if (m_window == NULL) {
        std::cout << "failed to create GLFW window\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD failed to initialize\n";
    }
    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}
void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}
void Window::pollEvents() {
    glfwPollEvents();
}

GLFWwindow* Window::getHandle() const {
    return m_window;
}
void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WindowContext* context = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));
    if (context && context->window) {
        context->window->m_width = width;
        context->window->m_height = height;
    }
}
