#pragma once

#include <eng.h>

class Bullet : public eng::GameObject
{
    GAMEOBJECT(Bullet)
    
public:
    void Update(float DeltaTime) override;
    
private:
    float m_lifeTime = 2.0f;
};
