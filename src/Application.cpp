#include <glad/glad.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "postprocessing.h"
#include <iostream>
#include "shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "stb_image.h"
#include <filesystem>
#include "Application.h"
#include "Bloom.h"
#include "model.h"
#include "Ssao.h"

void swapEffect(PostProcessing** currentEffect, PostProcessing* newEffect, string vertexShader, string fragmentShader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, bool gammaCorrection);
void ssao(unsigned int* gPositionptr, unsigned int* gNormalptr, unsigned int* gAlbedoptr, unsigned int* gBufferptr, unsigned int* ssaoFBOptr, unsigned int* ssaoBlurFBOptr, unsigned int* noiseTextureptr, Shader* shaderGeometryPass, Shader* shaderSSAO, Shader* shaderLightingPass, Shader* shaderSSAOBlur, std::vector<glm::vec3>* ssaoKernelptr, unsigned int* ssaoColorBuffer, unsigned int* ssaoColorBufferBlur);
void renderSceneBloom(Shader bloomShader, Shader shaderLight, Shader shaderBlur, Shader shaderBloomFinal, unsigned woodTexture, unsigned containerTexture, unsigned hdrFBO, unsigned colorBuffers[2], std::vector<glm::vec3> lightPositions, std::vector<glm::vec3> lightColors, unsigned pingpongFBO[2], unsigned pingpongColorbuffers[2], BloomRenderer bloomRenderer);
void renderCube();
void renderSceneSsao(Model* scene, Shader shaderGeometryPass, Shader shaderLightingPass, Shader shaderSSAO, Shader shaderSSAOBlur, unsigned gBuffer, unsigned ssaoFBO, unsigned ssaoBlurFBO, std::vector<glm::vec3> ssaoKernel, unsigned gPosition, unsigned gNormal, unsigned gAlbedo, unsigned ssaoColorBuffer, unsigned ssaoColorBufferBlur, unsigned noiseTexture, glm::vec3 lightPos, glm::vec3 lightColor, float ssaoRadius, float ssaoBias);
void setClearColor(float r, float g, float b);

using namespace glm;
using namespace std;

//set global variables
bool bloom = false;
bool dofBool = false;
int sceneNum = 1;
bool filmg = false;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;



int main(void)
{
    // glfw: initialize and configure
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 400";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Postprocessing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);


    // instanciate our shader programs and scenes
    Shader modelShader("shaders/model_loading.vert", "shaders/model_loading.frag");
	Model scene1("models/objects/winter/Low Poly Winter Scene.obj");
    Model scene2("models/objects/medieval/medieval.obj");
    Model scene3("models/objects/de_dust2/de_dust2.obj");
    Model scene4("models/objects/game/GameLevel.obj");
    Model scene5("models/objects/Fantasy Inn/Fantasy_inn.obj");
    Model*scene = &scene1;
    Shader shaderGeometryPass("shaders/ssao/ssao_geometry.vs", "shaders/ssao/ssao_geometry.fs");
    Shader shaderLightingPass("shaders/ssao/ssao.vs", "shaders/ssao/ssao_lighting.fs");
    Shader shaderSSAO("shaders/ssao/ssao.vs", "shaders/ssao/ssao.fs");
    Shader shaderSSAOBlur("shaders/ssao/ssao.vs", "shaders/ssao/ssao_blur.fs");
	Shader bloomShader("shaders/bloom/bloom.vs", "shaders/bloom/bloom.fs");
    Shader shaderLight("shaders/bloom/bloom.vs", "shaders/bloom/light_box.fs");
    Shader shaderBlur("shaders/bloom/old_blur.vs", "shaders/bloom/old_blur.fs");
    Shader shaderBloomFinal("shaders/bloom/bloom_final.vs", "shaders/bloom/bloom_final.fs");

    //load used textures
    unsigned int woodTexture = loadTexture("textures/clean.jpg", true); 
    unsigned int containerTexture = loadTexture("textures/clean.jpg", true);


    // configure (floating point) framebuffers for Bloom
    unsigned int hdrFBO;
    unsigned int colorBuffers[2];
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    bloomSetup(&hdrFBO,colorBuffers ,pingpongFBO, pingpongColorbuffers, &lightPositions, &lightColors, &bloomShader, &shaderBlur, &shaderBloomFinal);


    //configure Buffers and Textures for SSAO
    unsigned int gBuffer;
    unsigned int ssaoFBO;
	unsigned int ssaoBlurFBO;
    std::vector<glm::vec3> ssaoKernel;
    unsigned int gPosition, gNormal, gAlbedo;
    unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
    unsigned int noiseTexture;
	ssao(&gPosition, &gNormal, &gAlbedo, &gBuffer, &ssaoFBO, &ssaoBlurFBO, &noiseTexture, &shaderGeometryPass, &shaderSSAO, &shaderLightingPass, &shaderSSAOBlur, &ssaoKernel,&ssaoColorBuffer,&ssaoColorBufferBlur);
    glm::vec3 lightPos = glm::vec3(1.0, 3.0, 0.0);
    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);




    //instanciate all the post processing effects
    PostProcessing noEffect;
    PostProcessing inverseEffect;
    PostProcessing kernelWaveEffect;
    PostProcessing kernelEffect;
    PostProcessing kernelEffect2;
    PostProcessing vignetteEffect;
    PostProcessing waveEffect;
    PostProcessing pixelEffect;
    PostProcessing kuwahara;
    PostProcessing sharpen;
    PostProcessing boxBlur;
    PostProcessing chromaticAbberation;
    PostProcessing dithering;
    PostProcessing filmGrain;
    PostProcessing outline;
    PostProcessing lethal;
    PostProcessing posterization;

    //default effect ist noEffect
    noEffect.Initialize(SCR_WIDTH, SCR_HEIGHT, "shaders/raw_noeffect/postprocessing.vs", "shaders/raw_noeffect/postprocessing.fs");
    //set current effect as noEffect
    PostProcessing* currentEffect = &noEffect;

    // Setup IMGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    
    //ssao boolean and default parameters
    bool ssao = false;
    float ssaoRadius = 0.5f;
    float ssaoBias = 0.025f;

    //uniforms
    float filmGrainStrength = 0.1f;
    int pixelSize = 5;
    float sharpenStrength = 0.3;
    int lookupsize = 64;
    float errorCarry = 0.3;
    int outlineInt = 8;
    int blurSize = 8;
    int kernelCore = 10;
    int kernelCore2 = 3;
    int wave_core = 7;
    float lethalCore = 1.0f;
    //setup physical Bloom
    BloomRenderer bloomRenderer;
    bloomRenderer.Init(SCR_WIDTH, SCR_HEIGHT);
    bool switchBloom = true;

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //poll events, process input, clear screen and macht IMGUI frame
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        //start using post processing if current effect is not ssao
        if(!ssao) (*currentEffect).StartProcessing();
        //give time uniform
        (*currentEffect).setFloat("time",deltaTime);
        glEnable(GL_DEPTH_TEST); 

        glClearColor(clearcolor[0], clearcolor[1], clearcolor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //setup ImGui Window position, to the right of the screen
        ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH+200, (SCR_HEIGHT/2)-250), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Appearing);
        //begin ImGui Window
    	ImGui::Begin("Post Processing");                        
		ImGui::ColorEdit3("clear color",clearcolor);

        //ImGui Slider for the Scene switch, give different Parameters depending on the scene
        int oldscene = sceneNum;
        ImGui::SliderInt("scene", &sceneNum, 1, 5);
        if(oldscene != sceneNum)
        {
            camera.resetCameraPosition();
            switch (sceneNum)
            {
            case 1:
                setClearColor(0.421f, 0.664f, 0.672f);
                
                break;
            case 2:
                setClearColor(0.551f, 0.786f, 0.995f);
                break;
            case 3:
                setClearColor(0.877f, 0.770f, 0.473f);
                break;
            case 4:
                setClearColor(0.0f, 0.471f, 1.0f);
                break;
            case 5:
                setClearColor(0.789f, 0.762f, 0.607f);
                break;
            default:
                setClearColor(0, 0, 0);
            }
        }
        switch (sceneNum)
        {
	        case 1:
				scene = &scene1;
                rotation = 0.0f;
                scaleNum = 1.0f;
                modelpos = vec3(0.0f, -2.0f, -3.0f);
				break;
            case 2:
                scene = &scene2;
                rotation = 0.0f;
                scaleNum = 1.0f;
                modelpos = vec3(0.0f, 0.0f, 0.0f);
                break;
            case 3:
                scene = &scene3;
                rotation = 270.0f;
				scaleNum = 0.01f;
                modelpos = vec3(0.0f, 0.0f, -2.0f);
				break;
            case 4:
                scene = &scene4;
                rotation = 0.0f;
                scaleNum = 1.0f;
                modelpos = vec3(0.0f, -5.0f, -5.0f);
                break;
            case 5:
                scene = &scene5;
                rotation = 0.0f;
                scaleNum = 10.0f;
				modelpos = vec3(-30.0f, -5.0f, 0.0f);
                break;
            default:
                scene = &scene1;
        }
        //swap Effect if buttons are pressed and set parameters/uniforms for the effect
        if (ImGui::Button("no effect")) {
            swapEffect(&currentEffect, &noEffect, "shaders/raw_noeffect/postprocessing.vs", "shaders/raw_noeffect/postprocessing.fs");
        }
        if (ImGui::Button("inverse")) {
            swapEffect(&currentEffect, &inverseEffect, "shaders/inverse_effect/postprocessing.vs", "shaders/inverse_effect/postprocessing.fs");
        }
        if (ImGui::Button("Film Grain")) {
            swapEffect(&currentEffect, &filmGrain, "shaders/film_grain/postprocessing.vs", "shaders/film_grain/postprocessing.fs");
        }
        if(currentEffect == & filmGrain)
        {
        	ImGui::SliderFloat("strength", &filmGrainStrength, 0.0f, 1.0f);
			(*currentEffect).setFloat("strength",filmGrainStrength);
        }
        if (ImGui::Button("kernel and wave effect")) {
            swapEffect(&currentEffect, &kernelWaveEffect, "shaders/kernel_and_wave_effect/postprocessing.vs", "shaders/kernel_and_wave_effect/postprocessing.fs");
        }
        if (currentEffect == &kernelWaveEffect)
        {
            ImGui::SliderInt("Kernel Core", &wave_core, -30, 30);
            (*currentEffect).setInt("core", wave_core);
        }
        if (ImGui::Button("kernel effect")) {
            swapEffect(&currentEffect, &kernelEffect, "shaders/kernel_effect/postprocessing.vs", "shaders/kernel_effect/postprocessing.fs");
        }
        if (currentEffect == &kernelEffect)
        {
            ImGui::SliderInt("Kernel Core", &kernelCore, 0, 30);
            (*currentEffect).setInt("core", kernelCore);
        }
        if (ImGui::Button("kernel effect 2")) {
            swapEffect(&currentEffect, &kernelEffect2, "shaders/kernel_effect2/postprocessing.vs", "shaders/kernel_effect2/postprocessing.fs");
        }
        if (currentEffect == &kernelEffect2)
        {
            ImGui::SliderInt("Kernel Core", &kernelCore2, 0, 20);
            (*currentEffect).setInt("core", kernelCore2);
        }
        if (ImGui::Button("vignette effect")) {
            swapEffect(&currentEffect, &vignetteEffect, "shaders/vignette_effect/postprocessing.vs", "shaders/vignette_effect/postprocessing.fs");
        }
        if (ImGui::Button("wave effect")) {
            swapEffect(&currentEffect, &waveEffect, "shaders/wave_effect/postprocessing.vs", "shaders/wave_effect/postprocessing.fs");
        }
        if (ImGui::Button("Posterization")) {
            swapEffect(&currentEffect, &posterization, "shaders/posterize/postprocessing.vs", "shaders/posterize/postprocessing.fs");
        }
        if (ImGui::Button("Outline")) {
            swapEffect(&currentEffect, &outline, "shaders/outline/postprocessing.vs", "shaders/outline/postprocessing.fs");
        }
        if (currentEffect == &outline)
        {
            ImGui::SliderInt("Kernel Core", &outlineInt, 1, 30);
            (*currentEffect).setInt("outline", outlineInt);
        }
        if (ImGui::Button("pixel effect")) {
            swapEffect(&currentEffect, &pixelEffect, "shaders/pixelize/postprocessing.vs", "shaders/pixelize/postprocessing.fs");
        }
        if (currentEffect == &pixelEffect)
        {
        	ImGui::SliderInt("pixel size", &pixelSize, 1,10);
			(*currentEffect).setInt("pixelSize", pixelSize);
        }
        if (ImGui::Button("Lethal")) {
            swapEffect(&currentEffect, &lethal, "shaders/lethal/postprocessing.vs", "shaders/lethal/postprocessing.fs");
        }
        if (currentEffect == &lethal)
        {
            ImGui::SliderFloat("Kernel Core", &lethalCore, -10.0f, 10.0f);
            (*currentEffect).setFloat("core", lethalCore);
        }
        if (ImGui::Button("Kuwahara Filter")) {
            swapEffect(&currentEffect, &kuwahara, "shaders/kuwahara_filter/postprocessing.vs", "shaders/kuwahara_filter/postprocessing.fs");
        }
        if (ImGui::Button("Sharpen")) {
            swapEffect(&currentEffect, &sharpen, "shaders/sharpen/postprocessing.vs", "shaders/sharpen/postprocessing.fs");
        }
        if (currentEffect == &sharpen)
        {
            ImGui::SliderFloat("strength", &sharpenStrength, 0.0f, 1.0f);
            (*currentEffect).setFloat("amount", sharpenStrength);
        }
        if (ImGui::Button("Box Blur")) {
            swapEffect(&currentEffect, &boxBlur, "shaders/box_blur/postprocessing.vs", "shaders/box_blur/postprocessing.fs");
        }
        if (currentEffect == &boxBlur)
        {
            ImGui::SliderInt("Blur Strength", &blurSize, 1, 30);
            (*currentEffect).setInt("size", blurSize);
        }
        if (ImGui::Button("Chromatic Abberation")) {
            swapEffect(&currentEffect, &chromaticAbberation, "shaders/chromatic_abberation/postprocessing.vs", "shaders/chromatic_abberation/postprocessing.fs");
        }
        if (ImGui::Button("Dithering")) {
            swapEffect(&currentEffect, &dithering, "shaders/dithering/postprocessing.vs", "shaders/dithering/postprocessing.fs");
        }
        if (currentEffect == &dithering)
        {
            ImGui::SliderInt("Lookup Size", &lookupsize, 1, 128);
            (*currentEffect).setInt("lookupSize", lookupsize);
            ImGui::SliderFloat("error Carry", &errorCarry, 0.0f, 3.0f);
            (*currentEffect).setFloat("errorCarry", errorCarry);
        }
        //end  Effect if SSAO is ticked
        if (ImGui::Checkbox("SSAO mode", &ssao)) {
            (*currentEffect).EndProcessing();
        }
        //slider for radius and Bias 
        if (ssao)
        {
            ImGui::SliderFloat("radius", &ssaoRadius, 0.0f, 1.0f);
            ImGui::SliderFloat("bias", &ssaoBias, 0.0f, 1.0f);
        }
        //end all Effect if Bloom is ticked
        if (ImGui::Checkbox("Bloom", &bloom)) {
            (*currentEffect).EndProcessing();
            setClearColor(0, 0, 0);

        }
        //slider for exposure and filter radius and Bloom type
        if(bloom)
        {
            ImGui::SliderFloat("exposure", &exposure, 0.0f, 10.0f);
            ImGui::SliderFloat("Filter Radius", &bloomFilterRadius, 0.0f, 0.050f);
            ImGui::SliderInt("Bloom level", &programChoice, 1, 3);
            ImGui::Text("1.no Bloom       2.primitive Bloom       3.physical Bloom");
        }

    	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    	ImGui::End();
        
        //render the scene
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearcolor[0], clearcolor[1], clearcolor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        //If ssao is ticked, render the scene with ssao
        if(ssao)
        {
	        renderSceneSsao(scene, shaderGeometryPass, shaderLightingPass, shaderSSAO, shaderSSAOBlur, gBuffer, ssaoFBO,
	                        ssaoBlurFBO, ssaoKernel, gPosition, gNormal, gAlbedo, ssaoColorBuffer, ssaoColorBufferBlur,
	                        noiseTexture, lightPos, lightColor, ssaoRadius, ssaoBias);
            switchBloom = true;

        }else if(bloom) //If bloom is ticked, render the scene with bloom
        {
            if(switchBloom == true)
            {
                camera.Position = vec3(0,3.0,0);
                switchBloom = false;
            }
	        renderSceneBloom(bloomShader, shaderLight, shaderBlur, shaderBloomFinal, woodTexture, containerTexture, hdrFBO,
	                         colorBuffers, lightPositions, lightColors, pingpongFBO, pingpongColorbuffers, bloomRenderer);
        }
		else //Render Scene normally with Postprocessing effect
        {
            switchBloom = true;
            camera.ProcessMouseMovement(0, 0, false);
            glm::mat4 model = glm::mat4(1.0f);

            glm::mat4 view = camera.GetViewMatrix();
            camera.ProcessMouseMovement(0, 0, true);
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            modelShader.use();
            modelShader.setMat4("view", view);
            modelShader.setMat4("projection", projection);
            drawScene(scene, &modelShader, scaleNum, rotation,modelpos);
            (*currentEffect).EndProcessing();
        }

    	glfwSwapBuffers(window);

        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void setClearColor(float r, float g, float b)
{
	clearcolor[0] = r;
	clearcolor[1] = g;
	clearcolor[2] = b;
}

//Draws scene with given scale and rotation
void drawScene(Model* model,Shader* modelShader,float scale,float rotationnum,vec3 pos)
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat,pos);
    modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));	
    modelMat = glm::rotate(modelMat, glm::radians(rotationnum), glm::vec3(1.0f, 0.0f, 0.0f));
    (*modelShader).setMat4("model", modelMat);
    (*model).Draw((*modelShader));
}

//switch effect and initialize new effect
void swapEffect(PostProcessing** currentEffect, PostProcessing* newEffect,string vertexShader,string fragmentShader)
{
	(**currentEffect).EndProcessing();
    (*newEffect).Initialize(SCR_WIDTH, SCR_HEIGHT, vertexShader.c_str(), fragmentShader.c_str());
	*currentEffect = newEffect;
	(**currentEffect).StartProcessing();
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (exposure > 0.0f)
            exposure -= 0.001f;
        else
            exposure = 0.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        exposure += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        programChoice = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        programChoice = 2;
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        programChoice = 3;
    }
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
//load textures for HDR
unsigned int loadTexture(char const* path, bool gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


