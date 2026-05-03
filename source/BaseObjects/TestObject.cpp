#include "TestObject.h"
#include "GLFW/glfw3.h"

TestObject::TestObject()
{
    std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        
        uniform mat4 uModel;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = uModel * vec4(position, 1.0);
        }
    )";
	
    std::string FragmentShaderCode = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0f);
        }
    )";
	
    auto& GraphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI();
    auto ShaderProgram = GraphicsAPI.CreateShaderProgram(VertexShaderCode, FragmentShaderCode);
    auto material = std::make_shared<eng::Material>();
    material->SetShaderProgram(ShaderProgram);
	
    std::vector<float> vertices = 
    {
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices = 
    {
        0, 1, 3,
        1, 2, 3
    };
	
    eng::VertexLayout vertexLayout;
	
    // Position
    vertexLayout.elements.push_back
    ({
        0,
        3,
        GL_FLOAT,
        0
    });
	
    // Color
    vertexLayout.elements.push_back
    ({
        1,
        3,
        GL_FLOAT,
        sizeof(float) * 3
    });
	
    vertexLayout.stride = sizeof(float) * 6;
	auto mesh = std::make_shared<eng::Mesh>(vertexLayout, vertices, indices);
    
    AddComponent(new eng::MeshComponent(mesh, material));
}

void TestObject::Update(float DeltaTime)
{
    GameObject::Update(DeltaTime);
    
    auto Position = GetPosition();
    auto& input = eng::Engine::GetInstance().GetInputManager();
    // Horizontal movement
    if (input.IsKeyPressed(GLFW_KEY_A))
    {
        Position.x -= 0.01f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_D))
    {
        Position.x += 0.01f;
    }
    // Vertical movement
    if (input.IsKeyPressed(GLFW_KEY_W))
    {
        Position.y += 0.01f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_S))
    {
        Position.y -= 0.01f;
    }
    SetPosition(Position);
    
}
