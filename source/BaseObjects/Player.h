#pragma once
#include <eng.h>

class Player : public eng::GameObject
{
public:
    void Init();
    void Update(float DeltaTime) override;
    
private:
    eng::AnimationComponent* m_animationComponent = nullptr;
};
