#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public: 
    Window(int width, int height, const char* title);
    ~Window();
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    GLFWwindow* getHandle() const;

private:
    GLFWwindow* m_window;
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};