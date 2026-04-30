#include "GraphicsAPI.h"
#include "ShaderProgram.h"
#include "render/Material.h"
#include "render/Mesh.h"
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
        if (shaderProgram)
        {
            shaderProgram->Bind();   
        }
    }

    void GraphicsAPI::BindMaterial(Material* material)
    {
        if (material)
        {
            material->Bind();
        }
    }

    void GraphicsAPI::BindMesh(Mesh* mesh)
    {
        if (mesh)
        {
            mesh->Bind();
        }
    }

    void GraphicsAPI::DrawMesh(Mesh* mesh)
    {
        if (mesh)
        {
            mesh->Draw();
        }   
    }

    void GraphicsAPI::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void GraphicsAPI::ClearBuffers()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    GLuint GraphicsAPI::CreateVertexBuffer(const std::vector<float>& vector)
    {
        GLuint VBO = 0;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float), vector.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return VBO;
    }

    GLuint GraphicsAPI::CreateIndexBuffer(const std::vector<uint32_t>& indices)
    {
        GLuint EBO = 0;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return EBO;
    }
}
