#pragma once
#include "shader_s.h"
#include <glm/glm.hpp>
#include <filesystem>

void ssao(unsigned int* gPositionptr, unsigned int* gNormalptr, unsigned int* gAlbedoptr, unsigned int* gBufferptr, unsigned int* ssaoFBOptr, unsigned int* ssaoBlurFBOptr, unsigned int* noiseTextureptr, Shader* shaderGeometryPass, Shader* shaderSSAO, Shader* shaderLightingPass, Shader* shaderSSAOBlur, std::vector<glm::vec3>* ssaoKernelptr, unsigned int* ssaoColorBufferptr, unsigned int* ssaoColorBufferBlurptr);