#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vertex.h"

class Mesh {
public:
    Mesh(const Vertex* vertices, const size_t vertexCount, const GLuint* indices, const size_t indexCount);
    ~Mesh();
    void configureInstancing(const float* data, const size_t count);
    void updateInstanceData(const float* data, const size_t count);
    void drawInstances(GLenum mode, GLsizei count);
    void draw(GLenum mode);
private:
    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_EBO;
    GLsizei m_indexCount;
    GLuint m_instanceVBO;
    int m_numberOfValuesPerInstance = 5;
};
