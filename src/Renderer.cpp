#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

void drawCubeWithOutline(Window* window, Camera* camera, Shader* shader, Shader* outlineShader, Mesh* cubeMesh, Mesh* cubeOutline) {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // shader->useProgram();
    // shader->setMat4("view", camera->getViewMatrix());
    // shader->setMat4("projection", camera->getProjectionMatrix((float) window->getWidth() / window->getHeight()));
    // shader->setMat4("model", glm::mat4(1.0f));
    // cubeMesh->draw(GL_TRIANGLES);

    
    outlineShader->useProgram();
    outlineShader->setMat4("view", camera->getViewMatrix());
    outlineShader->setMat4("projection", camera->getProjectionMatrix((float) window->getWidth() / window->getHeight()));
    outlineShader->setMat4("model", glm::mat4(1.0f));
    glLineWidth(2.0f);
    cubeOutline->draw(GL_LINES);
}
void initRenderState() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}