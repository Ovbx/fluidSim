#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vertex.h"

namespace Square {
const Vertex vertices[] = {
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
const GLuint indices[] = {
    0,1,2, 2,3,0,   // back face
    4,5,6, 6,7,4,   // front face
    0,4,7, 7,3,0,   // left face
    1,5,6, 6,2,1,   // right face
    3,2,6, 6,7,3,   // top face
    0,1,5, 5,4,0    // bottom face
};
const GLuint edgeIndices[] = {
    // bottom face edges
    0,1, 1,2, 2,3, 3,0,
    // top face edges
    4,5, 5,6, 6,7, 7,4,
    // vertical edges connecting bottom to top
    0,4, 1,5, 2,6, 3,7
};
const size_t vertexCount = sizeof(vertices) / sizeof(vertices[0]);
const size_t cubeIndexCount = sizeof(indices) / sizeof(indices[0]);
const size_t cubeEdgeIndexCount = sizeof(edgeIndices) / sizeof(edgeIndices[0]);


}
namespace twoDimensionalArrow {
    const Vertex vertices[] = {
        //rectangle shaft
        {{0.0f, -0.2f, 0.0f}, {1.0f, 0.0f, 0.0f}}, //bottom left
        {{1.2f, -0.2f, 0.0f}, {1.0f, 0.0f, 0.0f}}, //bottom right
        {{1.2, 0.2f, 0.0f}, {1.0f, 0.0f, 0.0f}}, //top right
        {{0.0f, 0.2f, 0.0f}, {1.0f, 0.0f, 0.0f}}, //top left

        //triangle head
        {{1.2f, -0.6f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // bottom point
        {{1.2f, 0.6f, 0.0f}, {1.0f, 0.0f, 0.0f}}, //top point
        {{2.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, //pointy of the triangle

    };
    const GLuint indices[] {
        0, 1, 2, 
        3, 0, 2,
        4, 6, 5
        
    };
    const size_t vertexCount = sizeof(vertices) / sizeof(vertices[0]);
    const size_t indexCount = sizeof(indices) / sizeof(indices[0]);

}