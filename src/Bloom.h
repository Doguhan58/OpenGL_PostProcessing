#pragma once
#ifndef BLOOM_H
#define BLOOM_H

#include "shader_s.h"
#include <glm/glm.hpp>
#include <filesystem>
#include "Bloom.h"
void bloomSetup(unsigned int* hdrFBOptr, unsigned int* colorBuffersptr, unsigned int* pingpongFBOptr, unsigned int* pingpongColorbuffersptr, std::vector<glm::vec3>* lightPositionsptr, std::vector<glm::vec3>* lightColorsptr, Shader* shader, Shader* shaderBlur, Shader* shaderBloomFinal);
void renderQuad();


struct bloomMip
{
    glm::vec2 size;
    glm::ivec2 intSize;
    unsigned int texture;
};

class bloomFBO
{
public:
    bloomFBO();
    ~bloomFBO();
    bool Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength);
    void Destroy();
    void BindForWriting();
    const std::vector<bloomMip>& MipChain() const;

private:
    bool mInit;
    unsigned int mFBO;
    std::vector<bloomMip> mMipChain;
};


class BloomRenderer
{
public:
    BloomRenderer();
    ~BloomRenderer();
    bool Init(unsigned int windowWidth, unsigned int windowHeight);
    void Destroy();
    void RenderBloomTexture(unsigned int srcTexture, float filterRadius);
    unsigned int BloomTexture();
    unsigned int BloomMip_i(int index);

private:
    void RenderDownsamples(unsigned int srcTexture);
    void RenderUpsamples(float filterRadius);

    bool mInit;
    bloomFBO mFBO;
    glm::ivec2 mSrcViewportSize;
    glm::vec2 mSrcViewportSizeFloat;
    Shader* mDownsampleShader;
    Shader* mUpsampleShader;

    bool mKarisAverageOnDownsample = true;
};

#endif