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
	m_scene->CreateGameObject<TestObject>("TestObject");
	
    auto material = eng::Material::Load("materials/brick.mat");
	auto mesh = eng::Mesh::CreateCube();
	
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
	
	auto Light = m_scene->CreateGameObject("Light");
	auto LightComponent = new eng::LightComponent();
	LightComponent->SetColor(glm::vec3(1.0f));
	Light->AddComponent(LightComponent);
	Light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	
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
