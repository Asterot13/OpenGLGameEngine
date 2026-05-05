#include "Game.h"
#include "BaseObjects/TestObject.h"
#include "scene/components/CameraComponent.h"

bool Game::Init()
{
	m_scene = new eng::Scene();
	
	auto Camera = m_scene->CreateGameObject("Camera");
	Camera->AddComponent(new eng::CameraComponent());
	Camera->AddComponent(new eng::PlayerControllerComponent());
	Camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	
	
	m_scene->SetMainCamera(Camera);
	m_scene->CreateGameObject<TestObject>("TestObject");
	
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
