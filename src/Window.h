#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public: 
    Window(int width, int height, const char* title);
    ~Window();
    int getWidth() const {
        return m_width;
    }
    int getHeight() const {
        return m_height;
    }
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    GLFWwindow* getHandle() const;

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};