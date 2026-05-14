#include "Game.h"
#include "BaseObjects/TestObject.h"
#include "scene/components/CameraComponent.h"
#include <iostream>

bool Game::Init()
{
	m_scene = new eng::Scene();
	auto& fileSystem = eng::Engine::GetInstance().GetFileSystem();
	
	auto texture = eng::Texture::LoadFromFile("brick.png");
	
	auto Camera = m_scene->CreateGameObject("Camera");
	Camera->AddComponent(new eng::CameraComponent());
	Camera->AddComponent(new eng::PlayerControllerComponent());
	Camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	
	m_scene->SetMainCamera(Camera);
	//m_scene->CreateGameObject<TestObject>("TestObject");
	
    auto material = eng::Material::Load("materials/brick.mat");
	
    std::vector<float> vertices = 
    {
    	//front face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        
    	//top face
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    	
    	//right face
    	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    	
    	//left face
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    	
    	//bottom face
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    	
    	//back face
    	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
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
	
	// UV
	vertexLayout.elements.push_back
	({
		2,
		2,
		GL_FLOAT,
		sizeof(float) * 6
	});
	
    vertexLayout.stride = sizeof(float) * 8;
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
	
	auto SuzanneMesh = eng::Mesh::Load("models/Suzanne.gltf");
	auto SuzanneMaterial = eng::Material::Load("materials/suzanne.mat");
	
	auto SuzanneGameObject = m_scene->CreateGameObject("Suzanne");
	SuzanneGameObject->AddComponent(new eng::MeshComponent(SuzanneMesh, SuzanneMaterial));
	SuzanneGameObject->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	
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
