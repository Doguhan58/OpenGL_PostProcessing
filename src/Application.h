#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "camera.h"

//global Variables
class Model;
inline float clearcolor[3] = { 0.421f, 0.664f, 0.672f};
inline float exposure = 1.0f;
inline float bloomFilterRadius = 0.005f;
inline Camera camera(glm::vec3(1.0f, 3.0f, 6.0f));
inline int programChoice = 1;
inline float scaleNum = 1.0f;
inline float rotation = 0.0f;
inline glm::vec3 modelpos = glm::vec3(0.0f, 0.0f, 0.0f);
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;


void drawScene(Model* model, Shader* modelShader, float scale, float rotationnum, glm::vec3 pos);
#endif
