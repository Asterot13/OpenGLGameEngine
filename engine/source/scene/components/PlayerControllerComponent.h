#pragma once
#include "scene/Component.h"
#include "physics/KinematicCharacterController.h"
#include <memory>

namespace eng
{
    class PlayerControllerComponent : public Component
    {
        COMPONENT(PlayerControllerComponent)
        
    public:
        void Init() override;
        void Update(float DeltaTime) override;
        
    private:
        float m_sensitivity = 5.0f;
        float m_speed = 5.0f;
        float m_xRotation = 0.0f;
        float m_yRotation = 0.0f;
        
        std::unique_ptr<KinematicCharacterController> m_kinematicController;
    };
}
