#include "Mesh.h"
#include "graphics/GraphicsAPI.h"
#include "Engine.h"
#include <cgltf.h>

namespace eng
{
    Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
    {
        m_vertexLayout = layout;
        
        auto& GraphicsAPI = Engine::GetInstance().GetGraphicsAPI();
        m_VBO = GraphicsAPI.CreateVertexBuffer(vertices);
        m_EBO = GraphicsAPI.CreateIndexBuffer(indices);
        
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        for (auto& element : m_vertexLayout.elements)
        {
            glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE, m_vertexLayout.stride, (void*)(uintptr_t)element.offset);
            glEnableVertexAttribArray(element.index);
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        m_vertexCount = (vertices.size() * sizeof(float)) / layout.stride;
        m_indexCount = indices.size();
    }

    Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices)
    {
        m_vertexLayout = layout;
        
        auto& GraphicsAPI = Engine::GetInstance().GetGraphicsAPI();
        m_VBO = GraphicsAPI.CreateVertexBuffer(vertices);
        
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        for (auto& element : m_vertexLayout.elements)
        {
            glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE, m_vertexLayout.stride, (void*)(uintptr_t)element.offset);
            glEnableVertexAttribArray(element.index);
        }
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        m_vertexCount = (vertices.size() * sizeof(float)) / layout.stride;
    }

    Mesh::~Mesh()
    {
        if (m_EBO != 0)
        {
            glDeleteBuffers(1, &m_EBO);
        }

        if (m_VBO != 0)
        {
            glDeleteBuffers(1, &m_VBO);
        }

        if (m_VAO != 0)
        {
            glDeleteVertexArrays(1, &m_VAO);
        }
    }

    std::shared_ptr<Mesh> Mesh::CreateCube()
    {
        std::vector<float> vertices = 
        {
    	//front face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        
    	//top face
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    	
    	//right face
    	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    	
    	//left face
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    	
    	//bottom face
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    	
    	//back face
    	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f
        };

        std::vector<unsigned int> indices = 
        {
            //front face
            0, 1, 3,
            1, 2, 3,
            //top face
            4, 5, 6,
            4, 6, 7,
            // right face
            8, 9, 10,
            8, 10, 11,
            // left face
            12, 13, 14,
            12, 14, 15,
            // bottom face
            16, 17, 18,
            16, 18, 19,
            //back face
            20, 21, 22,
            20, 22, 23
        };
	
        eng::VertexLayout vertexLayout;
	    
        // Position
        vertexLayout.elements.push_back
        ({
            VertexElement::PositionIndex,
            3,
            GL_FLOAT,
            0
        });
	    
        // Color
        vertexLayout.elements.push_back
        ({
            VertexElement::ColorIndex,
            3,
            GL_FLOAT,
            sizeof(float) * 3
        });
	    
	    // UV
	    vertexLayout.elements.push_back
	    ({
		    VertexElement::UVIndex,
		    2,
		    GL_FLOAT,
		    sizeof(float) * 6
	    });

        // Normal
        vertexLayout.elements.push_back
        ({
            VertexElement::NormalIndex,
            3,
            GL_FLOAT,
            sizeof(float) * 8
        });
	    
        vertexLayout.stride = sizeof(float) * 11;
        
        auto result = std::make_shared<eng::Mesh>(vertexLayout, vertices, indices);
        return result;
    }

    void Mesh::Bind()
    {
        glBindVertexArray(m_VAO);
    }

    void Mesh::Draw()
    {
        if (m_indexCount > 0)
        {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount));
        }
    }
}
