#pragma once
#include <eng.h>

class Player : public eng::GameObject
{
    GAMEOBJECT(Player)
public:
    void Init() override;
    void Update(float DeltaTime) override;
    
private:
    eng::AnimationComponent* m_animationComponent = nullptr;
    eng::AudioComponent* m_audioComponent = nullptr;
    eng::PlayerControllerComponent* m_playerControllerComponent = nullptr;
};
