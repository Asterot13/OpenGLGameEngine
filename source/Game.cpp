#include "Game.h"
#include "BaseObjects/TestObject.h"

bool Game::Init()
{
	m_scene.CreateGameObject<TestObject>("TestObject");
	return true;
}

void Game::Update(float DeltaTime)
{
	m_scene.Update(DeltaTime);
}

void Game::Destroy()
{
}
