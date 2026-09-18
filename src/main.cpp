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
#include "meshShape.h"
#include "Fluid.h"
#include "tracy/Tracy.hpp"
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
const char* instancedVertexShaderSource = R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aColor;
  layout (location = 2) in vec4 instanceData;
  layout (location = 3) in float colorT;

  out vec3 ourColor;

  uniform mat4 view;
  uniform mat4 projection;

  void main () {
    vec3 blue = vec3(0, 0, 1);
    vec3 green = vec3(0, 1, 0);
    vec3 yellow = vec3(1, 1, 0);
    vec3 red = vec3(1, 0, 0);

    vec3 color;
    if (colorT < 0.33) {
      color = mix(blue, green, colorT / 0.33);
    }
    else if (colorT < 0.66) {
      color = mix(green, yellow, (colorT - 0.33) / 0.33);
    }
    else {
      color = mix(yellow, red, (colorT - 0.66) / 0.33);
    }
    ourColor = color;
    float angle = instanceData.z;
    float scale = instanceData.w;
    float cosOfInstanceData = cos(angle);
    float sinOfInstanceData = sin(angle);
    mat2 rotation = mat2(cosOfInstanceData, sinOfInstanceData, -sinOfInstanceData, cosOfInstanceData);
    vec2 rotatedPosition = (rotation * aPos.xy) * scale + instanceData.xy;
    gl_Position = projection * view * vec4(rotatedPosition, aPos.z, 1.0);
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
const char* arrowFillFragmentShaderSource = R"(
  #version 330 core
  out vec4 FragColor;
  in vec3 ourColor;

  void main() {
    FragColor = vec4(ourColor, 1.0f);
  }
)";
//variables
const float red = 0.0f;
const float green = 0.0f;
const float blue = 0.0f;
const float alpha = 1.0f;




//camera variables
glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
float distance = 10.0f;
float yaw = 45.0f;
float pitch = 0.0f;

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
        Square::vertices,
        Square::vertexCount,
        Square::indices,
        Square::cubeIndexCount
    );
    Mesh cubeOutline(
        Square::vertices,
        Square::vertexCount,
        Square::edgeIndices,
        Square::cubeEdgeIndexCount
    );
    Mesh arrow(
        twoDimensionalArrow::vertices,
        twoDimensionalArrow::vertexCount,
        twoDimensionalArrow::indices,
        twoDimensionalArrow::indexCount
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
    Shader arrowShader(
      vertexShaderSource,
      arrowFillFragmentShaderSource
    );
    Shader arrowInstancedShader(
      instancedVertexShaderSource,
      arrowFillFragmentShaderSource
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
    WindowContext context {
      &window,
      &camera
    };

    glfwSetWindowUserPointer(handle, &context);
    glfwSetCursorPosCallback(handle, mouseCallback);
    glfwSetScrollCallback(handle, scrollCallback);

    //StaggeredGrid
    int nx = 128;
    int ny = 128;
    double dt = 0.16;
    double gridSpacing = 1.0;
    StaggeredGrid grid(
      nx,
      ny,
      dt,
      gridSpacing
    );
    float worldSize = 1.0f;
    float minScale = 0.005f;
    float maxScale = 0.02f;
    int numberOfValuesPerInstance = 5;
    std::vector<float> initialData = grid.displaySolver(worldSize, minScale, maxScale);
    arrow.configureInstancing(initialData.data(), initialData.size() / numberOfValuesPerInstance);

    while(!window.shouldClose()) {
        processInput(handle);
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        grid.fluidSolver();
        std::vector<float> frameData = grid.displaySolver(worldSize, minScale, maxScale);
        arrow.updateInstanceData(frameData.data(), frameData.size() / numberOfValuesPerInstance);
        drawCubeWithOutline(&window, &camera, &shader, &outlineShader, &cubeMesh, &cubeOutline);
        drawArrowInstances(&window, &camera, &arrowInstancedShader, &arrow, nx * ny);

        window.swapBuffers();
        //profiling using tracy
        FrameMark;
        window.pollEvents();
    }

    return 0;
}
