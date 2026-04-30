#pragma once
#include <string>
#include <unordered_map>

#include "GL/glew.h"

namespace eng
{
    class ShaderProgram
    {
    public:
        explicit  ShaderProgram(GLuint ShaderProgramID);
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ~ShaderProgram();

        void Bind() const;
        GLint GetUniformLocation(const std::string& name);
        void SetUniform(const std::string& name, float value);
        
    private:
        std::unordered_map<std::string, GLint> m_UniformLocationCache;
        GLuint m_ShaderProgramID = 0;
    
    };
}
