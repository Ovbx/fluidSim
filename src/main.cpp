#include <filesystem>
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

int main(int argc, char ** argv)
{
  std::filesystem::path p1 = argv[0];
  std::filesystem::path shaderDir = p1.parent_path() / "shaders";
  std::cout << shaderDir << std::endl;
  std::filesystem::path meshVertexPath = shaderDir / "mesh.vert";
  std::filesystem::path meshFillPath = shaderDir / "meshFill.frag";
  std::filesystem::path meshOutlinePath = shaderDir / "meshOutline.frag";
  std::filesystem::path arrow2dVertexPath = shaderDir / "arrow2d.vert";
  std::filesystem::path arrow2dFragmentPath = shaderDir / "arrow2d.frag";
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
        meshVertexPath,
        meshFillPath
    );
    Shader outlineShader(
        meshVertexPath,
        meshOutlinePath
    );
    Shader arrowShader(
      meshVertexPath,
      arrow2dFragmentPath
    );
    Shader arrowInstancedShader(
      arrow2dVertexPath,
      arrow2dFragmentPath
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
    float maxScale = 0.018f;
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
