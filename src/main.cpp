#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include "Mesh.h"

//vertex stuff
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
}
)";
const char *fillFragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 ourColor;

    void main()
    {
        FragColor = vec4(ourColor, 0.4);
    }
)";

const char *outlineFragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
)";
//variables
float r = 0.0f;
float storedR = r;

Vertex vertices[] = {
    // positions                 // colors
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 1
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 2
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}, // 3
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}}, // 4
    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}}, // 5
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 6
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}}  // 7
};

GLuint indices[] = {
    0,1,2, 2,3,0,   // back face
    4,5,6, 6,7,4,   // front face
    0,4,7, 7,3,0,   // left face
    1,5,6, 6,2,1,   // right face
    3,2,6, 6,7,3,   // top face
    0,1,5, 5,4,0    // bottom face
};
GLuint edgeIndices[] = {
    // bottom face edges
    0,1, 1,2, 2,3, 3,0,
    // top face edges
    4,5, 5,6, 6,7, 7,4,
    // vertical edges connecting bottom to top
    0,4, 1,5, 2,6, 3,7
};

//camera variables
glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
float distance = 10.0f;
float yaw = 45.0f;
float pitch = 0.0f;


void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        std::cout << "clicked\n";
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        r = storedR;
    }
}
static void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
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
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        cam->processMouseScroll(yOffset);
    }
}

int main()
{
    //creates window: width, height, name, monitor, share
    Window window(
        1280,
        1080,
        "OPENGL TEST"
    );
    //cube
    Mesh cubeMesh(
        vertices,
        8,
        indices,
        36
    );
    Mesh cubeOutline(
        vertices, 
        8,
        edgeIndices,
        24
    );
    //shaders
    Shader shader(
        vertexShaderSource,
        fillFragmentShaderSource
    );
    Shader outlineShader(
        vertexShaderSource,
        outlineFragmentShaderSource
    );

    shader.useProgram();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //camera
    Camera camera(
        target,
        distance,
        yaw,
        pitch  
    );
    GLFWwindow* handle = window.getHandle();

    glfwSetWindowUserPointer(handle, &camera);
    glfwSetCursorPosCallback(handle, mouseCallback);
    glfwSetScrollCallback(handle, scrollCallback);

    while(!window.shouldClose()) {
        processInput(handle);
        glClearColor(r, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader.useProgram();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix((float) window.getWidth() / window.getHeight()));
        shader.setMat4("model", glm::mat4(1.0f));
        cubeMesh.draw(GL_FILL);

        
        outlineShader.useProgram();
        outlineShader.setMat4("view", camera.getViewMatrix());
        outlineShader.setMat4("projection", camera.getProjectionMatrix((float) window.getWidth() / window.getHeight()));
        outlineShader.setMat4("model", glm::mat4(1.0f));
        glLineWidth(2.0f);
        cubeOutline.draw(GL_LINES);

        

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}