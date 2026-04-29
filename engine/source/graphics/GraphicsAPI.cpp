#include "GraphicsAPI.h"
#include "ShaderProgram.h"
#include <iostream>

namespace eng
{
    std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexShaderSource,
                                                                    const std::string& fragmentShaderSource)
    {
        // Creating and compiling vector shader
        GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* VertexShaderCStr = vertexShaderSource.c_str();
        glShaderSource(VertexShader, 1, &VertexShaderCStr, nullptr);
        glCompileShader(VertexShader);

        GLint success;
        glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(VertexShader, 512, nullptr, infoLog);
            std::cerr << "Failed to compile vertex shader: " << infoLog << '\n';
            return nullptr;
        }
        
        // Creating and compiling fragment shader
        GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* FragmentShaderCStr = fragmentShaderSource.c_str();
        glShaderSource(FragmentShader, 1, &FragmentShaderCStr, nullptr);
        glCompileShader(FragmentShader);

        glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(FragmentShader, 512, nullptr, infoLog);
            std::cerr << "Failed to compile fragment shader: " << infoLog << '\n';
            return nullptr;
        }

        // Creating shader program ID
        GLuint ShaderProgramID = glCreateProgram();
        glAttachShader(ShaderProgramID, VertexShader);
        glAttachShader(ShaderProgramID, FragmentShader);
        glLinkProgram(ShaderProgramID);

        glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(ShaderProgramID, 512, nullptr, infoLog);
            std::cerr << "Failed to link shader program: " << infoLog << '\n';
            return nullptr;
        }
        
        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        
        return std::make_shared<ShaderProgram>(ShaderProgramID);
    }

    void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
    {
        shaderProgram->Bind();
    }
}
