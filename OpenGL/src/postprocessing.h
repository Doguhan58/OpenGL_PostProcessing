#pragma once

#include <glad/glad.h>

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class PostProcessing_Shader
{
private:
	GLuint shader;

	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void compileShaders(const char* vShaderCode, const char* fShaderCode);
public:
	PostProcessing_Shader();

    // Read a shader file (e.g. vertex shader)
	static std::string ReadShader(std::string fileName);

	// Create a shader program using vertex and fragment shaders
	void CreateProgram(std::string vShader, std::string fShader);

    GLuint getShader() { return this->shader; }

	// Using a shader program
	void UseShader() { glUseProgram(this->shader); }

	GLuint GetProgram() { return this->shader; }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(shader, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(shader, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

	~PostProcessing_Shader();
};

class PostProcessing
{
private:
    int width, height;

    unsigned int FBO;
    unsigned int framebufferTexture;

    GLuint uniformScreenResolution;

    static const float rectangleVerts[];

    unsigned int rectVAO;
public:
    PostProcessing();

    PostProcessing_Shader* framebuffer_shader;

    void Initialize(int _width, int _height, const char* vShader_Path, const char* fShader_Path);

    void StartProcessing();
    void EndProcessing();

    void UseShader() { glUseProgram(framebuffer_shader->getShader()); }

    GLuint GetProgram() { return framebuffer_shader->getShader(); }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(framebuffer_shader->getShader(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    ~PostProcessing();

};