#pragma once

#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

void drawCubeWithOutline(Window* window, Camera* camera, Shader* shader, Shader* outlineShader, Mesh* cubeMesh, Mesh* cubeOutline);
void initRenderState();