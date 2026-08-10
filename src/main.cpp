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
#include "Renderer.h"
#include "Input.h"

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
        FragColor = vec4(ourColor, 0.1f);
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
const float red = 0.0f;
const float green = 0.0f;
const float blue = 0.0f;
const float alpha = 1.0f;


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

//cube
const int cubeVertices = 8;
const int cubeIndexCount = 36;
const int cubeEdgeIndexCount = 24;


int main()
{
    //creates window: width, height, name, monitor, share
    int width = 1280;
    int height = 1080;
    
    Window window(
        width,
        height,
        "OPENGL TEST"
    );

    Mesh cubeMesh(
        vertices,
        cubeVertices,
        indices,
        cubeIndexCount
    );
    Mesh cubeOutline(
        vertices, 
        cubeVertices,
        edgeIndices,
        cubeEdgeIndexCount
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

    //rendering
    initRenderState();

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
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawCubeWithOutline(&window, &camera, &shader, &outlineShader, &cubeMesh, &cubeOutline);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}