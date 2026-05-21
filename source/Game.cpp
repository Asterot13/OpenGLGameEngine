#include "Game.h"
#include "BaseObjects/TestObject.h"
#include "scene/components/CameraComponent.h"
#include <iostream>

bool Game::Init()
{
	m_scene = new eng::Scene();
	eng::Engine::GetInstance().SetScene(m_scene);
	auto& fileSystem = eng::Engine::GetInstance().GetFileSystem();
	
	auto texture = eng::Texture::LoadFromFile("brick.png");
	
	auto Camera = m_scene->CreateGameObject("Camera");
	Camera->AddComponent(new eng::CameraComponent());
	Camera->AddComponent(new eng::PlayerControllerComponent());
	Camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	
	m_scene->SetMainCamera(Camera);
	m_scene->CreateGameObject<TestObject>("TestObject");
	
    auto material = eng::Material::Load("materials/brick.mat");
	auto mesh = eng::Mesh::CreateBox();
	
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
	
	auto SuzanneGameObject = eng::GameObject::LoadGLTF("models/Suzanne/Suzanne.gltf");
	SuzanneGameObject->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	
	auto Gun = eng::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
	Gun->SetParent(Camera);
	Gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
	Gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));
	
	if (auto animComp = Gun->GetComponent<eng::AnimationComponent>())
	{
		if (auto bullet = Gun->FindChildByName("bullet_33"))
		{
			bullet->SetActive(false);
		}
		
		if (auto fire = Gun->FindChildByName("BOOM_35"))
		{
			fire->SetActive(false);
		}
		
		animComp->Play("shoot");
	}
	
	auto Light = m_scene->CreateGameObject("Light");
	auto LightComponent = new eng::LightComponent();
	LightComponent->SetColor(glm::vec3(1.0f));
	Light->AddComponent(LightComponent);
	Light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	
	auto ground = m_scene->CreateGameObject("Ground");
	ground->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	
	glm::vec3 groundExtents = glm::vec3(20.0f, 2.0f, 20.0f);
	auto groundMesh = eng::Mesh::CreateBox(groundExtents);
	ground->AddComponent(new eng::MeshComponent(groundMesh, material));
	
	auto groundCollider = std::make_shared<eng::BoxCollider>(groundExtents);
	auto groundBody = std::make_shared<eng::RigidBody>(eng::RigidBodyType::Static, groundCollider, 0.0f, 0.5f);
	ground->AddComponent(new eng::PhysicsComponent(groundBody));
	
	auto boxObject = m_scene->CreateGameObject("FallingBox");
	boxObject->AddComponent(new eng::MeshComponent(mesh, material));
	boxObject->SetPosition(glm::vec3(0.0f, 5.0f, -5.0f));
	boxObject->SetRotation(glm::quat(0.0f, 1.0f, 0.0f, 1.57f));
	auto boxCollider = std::make_shared<eng::BoxCollider>(glm::vec3(1.0f));
	auto boxBody = std::make_shared<eng::RigidBody>(eng::RigidBodyType::Dynamic, boxCollider, 5.0f, 0.5f);
	boxObject->AddComponent(new eng::PhysicsComponent(boxBody));
	
	Camera->SetPosition(glm::vec3(0.0f, 1.0f, 7.0f));
	
	return true;
}

void Game::Update(float DeltaTime)
{
	m_scene->Update(DeltaTime);
}

void Game::Destroy()
{
}
