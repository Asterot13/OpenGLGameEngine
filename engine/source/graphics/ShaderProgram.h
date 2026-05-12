#pragma once
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include "GL/glew.h"

namespace eng
{
    class Texture;
    class ShaderProgram
    {
    public:
        explicit  ShaderProgram(GLuint ShaderProgramID);
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ~ShaderProgram();

        void Bind();
        GLint GetUniformLocation(const std::string& name);
        void SetUniform(const std::string& name, float value);
        void SetUniform(const std::string& name, float v0, float v1);
        void SetUniform(const std::string& name, glm::mat4 matrix);
        void SetTexture(const std::string& name, Texture* texture);
        
    private:
        std::unordered_map<std::string, GLint> m_UniformLocationCache;
        GLuint m_ShaderProgramID = 0;
        int m_currentTextureUnit = 0;
    };
}
