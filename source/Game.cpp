#include "Game.h"
#include "BaseObjects/TestObject.h"
#include "scene/components/CameraComponent.h"
#include <iostream>
#include <stb_image.h>

bool Game::Init()
{
	auto& fileSystem = eng::Engine::GetInstance().GetFileSystem();
	auto path = fileSystem.GetAssetsFolder() / "brick.png";
	
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		std::cout << "Loaded texture: " << path.string() << std::endl;
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture: " << path.string()
			<< " (" << stbi_failure_reason() << ")" << std::endl;
	}
	
	m_scene = new eng::Scene();
	
	auto Camera = m_scene->CreateGameObject("Camera");
	Camera->AddComponent(new eng::CameraComponent());
	Camera->AddComponent(new eng::PlayerControllerComponent());
	Camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	
	m_scene->SetMainCamera(Camera);
	m_scene->CreateGameObject<TestObject>("TestObject");
	
	    std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = uProjection * uView * uModel * vec4(position, 1.0); //MVP Transformation (Model, View, Projection)
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
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices = 
    {
        //front face
        0, 1, 3,
        1, 2, 3,
        //top face
        4, 5, 1,
        4, 1, 0,
        // right face
        4, 0, 3,
        4, 3, 7,
        // left face
        1, 5, 6,
        1, 6, 2,
        // bottom face
        3, 2, 6,
        3, 6, 7,
        //back face
        4, 7, 6,
        4, 6, 5
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
	
	auto GameObjectA = m_scene->CreateGameObject("Object A");
	GameObjectA->AddComponent(new eng::MeshComponent(mesh, material));
	GameObjectA->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
	
	auto GameObjectB = m_scene->CreateGameObject("Object B");
	GameObjectB->AddComponent(new eng::MeshComponent(mesh, material));
	GameObjectB->SetPosition(glm::vec3(0.0f, 2.0f, 3.0f));
	GameObjectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));
	
	auto GameObjectC = m_scene->CreateGameObject("Object C");
	GameObjectC->AddComponent(new eng::MeshComponent(mesh, material));
	GameObjectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	GameObjectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
	GameObjectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
	
	eng::Engine::GetInstance().SetScene(m_scene);
	return true;
}

void Game::Update(float DeltaTime)
{
	m_scene->Update(DeltaTime);
}

void Game::Destroy()
{
}
