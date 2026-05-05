#pragma once
#include "scene/Component.h"

namespace eng
{
    class PlayerControllerComponent : public Component
    {
        COMPONENT(PlayerControllerComponent)
        
    public:
        void Update(float DeltaTime) override;
        
    private:
        float m_sensitivity = 0.3f;
        float m_speed = 5.0f;
    };
}
