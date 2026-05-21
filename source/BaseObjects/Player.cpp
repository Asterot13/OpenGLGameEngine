#include "Player.h"
#include "GLFW/glfw3.h"

void Player::Init()
{
    AddComponent(new eng::CameraComponent());
    AddComponent(new eng::PlayerControllerComponent());
    SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    
    auto Gun = eng::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    Gun->SetParent(this);
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
    }
    
    m_animationComponent = Gun->GetComponent<eng::AnimationComponent>();
}

void Player::Update(float DeltaTime)
{
    GameObject::Update(DeltaTime);
    
    auto& input = eng::Engine::GetInstance().GetInputManager();
    if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        if (m_animationComponent && !m_animationComponent->IsPlaying())
        {
            m_animationComponent->Play("shoot", false);
        }
    }
}
