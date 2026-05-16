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

    // std::shared_ptr<Mesh> Mesh::Load(const std::string& path)
    // {
    //     auto content = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
    //     
    //     if (content.empty())
    //     {
    //         return nullptr;
    //     }
    //     
    //     auto readFloats = [](const cgltf_accessor* acc, cgltf_size i, float* out, int n)
    //     {
    //         std::fill(out, out + n, 0.0f);
    //         return cgltf_accessor_read_float(acc, i, out, n) == 1;
    //     };
    //     
    //     auto readIndices = [](const cgltf_accessor* acc, cgltf_size i)
    //     {
    //         cgltf_uint out = 0;
    //         cgltf_bool success = cgltf_accessor_read_uint(acc, i, &out, 1);
    //         return success ? static_cast<uint32_t>(out) : 0;
    //     };
    //     
    //     cgltf_options options = {};
    //     cgltf_data* data = nullptr;
    //     cgltf_result result = cgltf_parse(&options, content.data(), content.size(), &data);
    //     if (result != cgltf_result_success)
    //     {
    //         return nullptr;
    //     }
    //     
    //     auto fullPath = Engine::GetInstance().GetFileSystem().GetAssetsFolder() / path;
    //     result = cgltf_load_buffers(&options, data, fullPath.remove_filename().string().c_str());
    //     if (result != cgltf_result_success)
    //     {
    //         cgltf_free(data);
    //         return nullptr;
    //     }
    //     
    //     std::shared_ptr<Mesh> resultMesh = nullptr;
    //     for (cgltf_size meshIndex = 0; meshIndex < data->meshes_count; ++meshIndex)
    //     {
    //         auto mesh = data->meshes[meshIndex];
    //         for (cgltf_size primIndex = 0; primIndex < mesh.primitives_count; ++primIndex)
    //         {
    //             auto primitive = mesh.primitives[primIndex];
    //             if (primitive.type != cgltf_primitive_type_triangles)
    //             {
    //                 continue;
    //             }
    //             
    //             VertexLayout layout;
    //             cgltf_accessor* accessor[4] = {nullptr, nullptr, nullptr, nullptr};
    //             
    //             for (cgltf_size attIndex = 0; attIndex < primitive.attributes_count; ++attIndex)
    //             {
    //                 auto& attribute = primitive.attributes[attIndex];
    //                 auto acc = attribute.data;
    //                 if (!acc)
    //                 {
    //                     continue;
    //                 }
    //                 
    //                 VertexElement element;
    //                 element.type = GL_FLOAT;
    //                 
    //                 switch (attribute.type)
    //                 {
    //                     case cgltf_attribute_type_position:
    //                         {
    //                             accessor[VertexElement::PositionIndex] = acc;
    //                             element.index = VertexElement::PositionIndex;
    //                             element.size = 3;
    //                             break;
    //                         }
    //                     case cgltf_attribute_type_color:
    //                         {
    //                             if (attribute.index != 0)
    //                             {
    //                                 continue;
    //                             }
    //                             accessor[VertexElement::ColorIndex] = acc;
    //                             element.index = VertexElement::ColorIndex;
    //                             element.size = 3;
    //                             break;
    //                         }
    //                     case cgltf_attribute_type_texcoord:
    //                         {
    //                             if (attribute.index != 0)
    //                             {
    //                                 continue;
    //                             }
    //                             accessor[VertexElement::UVIndex] = acc;
    //                             element.index = VertexElement::UVIndex;
    //                             element.size = 2;
    //                             break;
    //                         }
    //                     case cgltf_attribute_type_normal:
    //                         {
    //                             accessor[VertexElement::NormalIndex] = acc;
    //                             element.index = VertexElement::NormalIndex;
    //                             element.size = 3;
    //                             break;
    //                         }
    //                     default:
    //                         continue;
    //                 }
    //                 
    //                 if (element.size > 0)
    //                 {
    //                     element.offset = layout.stride;
    //                     layout.stride += element.size * sizeof(float);
    //                     layout.elements.push_back(element);
    //                 }
    //             }
    //             
    //             if (!accessor[VertexElement::PositionIndex])
    //             {
    //                 continue;
    //             }
    //             
    //             auto vertexCount = accessor[VertexElement::PositionIndex]->count;
    //             std::vector<float> vertices;
    //             vertices.resize(layout.stride / sizeof(float) * vertexCount);
    //             
    //             for (cgltf_size vi = 0; vi < vertexCount; ++vi)
    //             {
    //                 for (auto& element : layout.elements)
    //                 {
    //                     if (!accessor[element.index])
    //                     {
    //                         continue;
    //                     }
    //                     
    //                     auto index = (vi * layout.stride + element.offset) / sizeof(float);
    //                     float* outData = &vertices[index];
    //                     readFloats(accessor[element.index], vi, outData, element.size);
    //                 }
    //             }
    //             
    //             if (primitive.indices)
    //             {
    //                 auto indexCount = primitive.indices->count;
    //                 std::vector<uint32_t> indices;
    //                 indices.resize(indexCount);
    //                 
    //                 for (cgltf_size i = 0; i < indexCount; ++i)
    //                 {
    //                     indices[i] = readIndices(primitive.indices, i);
    //                 }
    //                 
    //                 resultMesh = std::make_shared<Mesh>(layout, vertices, indices);
    //             }
    //             else
    //             {
    //                 resultMesh = std::make_shared<Mesh>(layout, vertices);
    //             }
    //             
    //             if (resultMesh)
    //             {
    //                 break;
    //             }
    //         }
    //         
    //         if (resultMesh)
    //         {
    //             break;
    //         }
    //     }
    //     
    //     cgltf_free(data);
    //     return resultMesh;
    // }

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
