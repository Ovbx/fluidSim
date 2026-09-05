#pragma once
#include <glm/glm.hpp>
#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

void drawCubeWithOutline(Window* window, Camera* camera, Shader* shader, Shader* outlineShader, Mesh* cubeMesh, Mesh* cubeOutline);
void drawArrow(Window* window, Camera* camera, Shader* shader, Mesh* arrowMesh, const glm::mat4& modelMatrix);
void initRenderState();