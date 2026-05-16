#include "GraphicsAPI.h"
#include "ShaderProgram.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include <iostream>

namespace eng
{
    bool GraphicsAPI::Init()
    {
        glEnable(GL_DEPTH_TEST);
        return true;
    }

    std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexShaderSource,
                                                                    const std::string& fragmentShaderSource)
    {
        // Creating and compiling vertex shader
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefaultShaderProgram()
    {
        if (!m_defaultShaderProgram)
        {
            std::string vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 color;
            layout (location = 2) in vec2 uv;
            layout (location = 3) in vec3 normal;

            uniform mat4 uModel;
            uniform mat4 uView;
            uniform mat4 uProjection;

            out vec2 vUV;
            out vec3 vNormal;
            out vec3 vFragPos;

            void main()
            {
                vUV = uv;;
                vFragPos = vec3(uModel * vec4(position, 1.0));
                vNormal = mat3(transpose(inverse(uModel))) * normal;

                gl_Position = uProjection * uView * uModel * vec4(position, 1.0); //MVP Transformation (Model, View, Projection)
            })";
            
            std::string fragmentShaderSource = R"(
            #version 330 core

            struct Light
            {
                vec3 position;
                vec3 color;
            };

            uniform Light uLight;

            out vec4 FragColor;

            in vec2 vUV;
            in vec3 vNormal;
            in vec3 vFragPos;

            uniform sampler2D baseColorTexture;

            void main()
            {
                // Calculate diffuse lighting
                vec3 normal = normalize(vNormal);
                vec3 lightDir = normalize(uLight.position - vFragPos);
                float diff = max(dot(normal, lightDir), 0.0);
                vec3 diffuse = diff * uLight.color;    

                //Apply texture with diffuse lighting
                vec4 texColor = texture(baseColorTexture, vUV);
                FragColor = texColor * vec4(diffuse, 1.0);
            })";
            
            m_defaultShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }
        
        return m_defaultShaderProgram;
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
