#pragma once
#include "GL/glew.h"
#include <memory>
#include <string>
#include <vector>

namespace eng
{
    class ShaderProgram;
    class Material;
    class Mesh;
    
    class GraphicsAPI
    {
    public:
        bool Init();
        std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        
        void BindShaderProgram(ShaderProgram* shaderProgram);
        void BindMaterial(Material* material);
        void BindMesh(Mesh* mesh);
        void DrawMesh(Mesh* mesh);
        
        void SetClearColor(float r, float g, float b, float a);
        void ClearBuffers();
        
        GLuint CreateVertexBuffer(const std::vector<float>& vector);
        GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);
    };
}
