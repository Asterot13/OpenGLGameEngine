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

    void GraphicsAPI::UnbindMesh(Mesh* mesh)
    {
        if (mesh)
        {
            mesh->Unbind();
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

    void GraphicsAPI::SetDepthTestEnabled(bool enable)
    {
        if (enable)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void GraphicsAPI::SetBlendMode(BlendMode blendMode)
    {
        switch (blendMode)
        {
        case BlendMode::Disabled:
            {
                glDisable(GL_BLEND);
            }
        break;
        case BlendMode::Alpha:
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
        break;
        case BlendMode::Additive:
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
            }
        break;
        case BlendMode::Multiply:
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_DST_COLOR, GL_ZERO);
            }
        break;
        default:
            glDisable(GL_BLEND);
            break;
        }
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
                vec3 direction;
                vec3 color;
            };

            uniform Light uLight;
            uniform vec3 uCameraPosition;

            out vec4 FragColor;

            in vec2 vUV;
            in vec3 vNormal;
            in vec3 vFragPos;

            uniform sampler2D baseColorTexture;

            void main()
            {
                vec3 normal = normalize(vNormal);

                // Calculate diffuse lighting
                vec3 lightDir = normalize(-uLight.direction);
                float diff = max(dot(normal, lightDir), 0.0);
                vec3 diffuse = diff * uLight.color;

	            // Calculate specular lighting
	            vec3 viewDir = normalize(uCameraPosition - vFragPos);
	            vec3 reflectDir = reflect(-lightDir, normal);
	            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	            float specularStrength = 0.5f;
	            vec3 specular = specularStrength * spec * uLight.color;

	            // Ambient Light
                const float ambientStrength = 0.4f;
                vec3 ambient = ambientStrength * uLight.color;
    

	            vec3 result = diffuse + specular + ambient;

                //Apply texture with lightning
	            vec4 texColor = texture(baseColorTexture, vUV);
                FragColor = texColor * vec4(result, 1.0);
            })";
            
            m_defaultShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }
        
        return m_defaultShaderProgram;
    }

    const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefault2DShaderProgram()
    {
        if (!m_default2DshaderProgram)
        {
            std::string vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec2 position;

            uniform mat4 uModel;
            uniform mat4 uView;
            uniform mat4 uProjection;
            uniform vec2 uPivot;
            uniform vec2 uSize;
            uniform vec2 uUVMin;
            uniform vec2 uUVMax;

            out vec2 vUV;

            void main()
            {
                vec2 local = (position - uPivot) * uSize;
                vUV = mix(uUVMin, uUVMax, position);

                gl_Position = uProjection * uView * uModel * vec4(local, 0.0, 1.0);
            })";
            
            std::string fragmentShaderSource = R"(
            #version 330 core

            in vec2 vUV;

            uniform vec4 uColor;
            uniform sampler2D uTex;

            out vec4 FragColor;

            void main()
            {
                vec4 src = texture(uTex, vUV) * uColor;
                FragColor = src;
            })";
            
            m_default2DshaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }
        return m_default2DshaderProgram;
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
