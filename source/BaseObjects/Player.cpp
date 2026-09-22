#include "Player.h"
#include "GLFW/glfw3.h"

void Player::Init()
{
    if (auto bullet = FindChildByName("bullet_33"))
    {
        bullet->SetActive(false);
    }
		
    if (auto fire = FindChildByName("BOOM_35"))
    {
        fire->SetActive(false);
    }
    
    if (auto Gun = FindChildByName("Gun"))
    {
        m_animationComponent = Gun->GetComponent<eng::AnimationComponent>();
    }
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
