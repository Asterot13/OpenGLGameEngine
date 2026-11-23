#include "Bullet.h"

void Bullet::Update(float DeltaTime)
{
    GameObject::Update(DeltaTime);
    
    m_lifeTime -= DeltaTime;
    if (m_lifeTime <= 0.0f)
    {
        MarkForDestroy();
    }
}
