#include "Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vertex.h"

Mesh::Mesh(const Vertex* vertices, const size_t vertexCount, const GLuint* indices, const size_t indexCount) : m_indexCount(indexCount) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glBufferData(GL_ARRAY_BUFFER, vertexCount* sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );
    glEnableVertexAttribArray(1);
}
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}
void Mesh::configureInstancing(const float* instanceData, const size_t instanceCount) {
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

  glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(float) * m_numberOfValuesPerInstance, instanceData, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(
    2,
    4,
    GL_FLOAT,
    GL_FALSE,
    sizeof(float) * m_numberOfValuesPerInstance,
    (void*) 0
  );
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glVertexAttribPointer(
    3,
    1,
    GL_FLOAT,
    GL_FALSE,
    sizeof(float) * m_numberOfValuesPerInstance,
    (void*)(sizeof(float) * 4)
  );
  glEnableVertexAttribArray(3);
  glVertexAttribDivisor(3, 1);
}
void Mesh::updateInstanceData(const float* data, const size_t count) {

  glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float) * m_numberOfValuesPerInstance, data);
}
void Mesh::drawInstances(GLenum mode, GLsizei instanceCount) {
  glBindVertexArray(m_VAO);
  glDrawElementsInstanced(mode, m_indexCount, GL_UNSIGNED_INT, 0, instanceCount);

}
void Mesh::draw(GLenum mode) {
    glBindVertexArray(m_VAO);
    glDrawElements(mode, m_indexCount, GL_UNSIGNED_INT, 0);
}
