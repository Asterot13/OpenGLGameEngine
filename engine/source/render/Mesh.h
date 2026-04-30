#pragma once
#include <GL/glew.h>
#include <cstdint>
#include <vector>
#include "graphics/VertexLayout.h"

namespace eng
{
    class Mesh
    {
    public:
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices);
        Mesh (const Mesh&) = delete;
        Mesh& operator= (const Mesh&) = delete;
        ~Mesh();
        
        void Bind();
        void Draw();
        
    private:
        VertexLayout m_vertexLayout;
        
        GLuint m_VBO = 0; // vertex buffer object
        GLuint m_EBO = 0; // element buffer object (indices)
        GLuint m_VAO = 0; // vertex array object
        
        size_t m_vertexCount = 0;
        size_t m_indexCount = 0;
    };
}

