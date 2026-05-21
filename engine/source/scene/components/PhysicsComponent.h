#pragma once
#include <memory>
#include "physics/RigidBody.h"
#include "scene/Component.h"

namespace eng
{
    class PhysicsComponent : public Component
    {
        COMPONENT(PhysicsComponent)
        
    public:
        PhysicsComponent() = default;
        PhysicsComponent(const std::shared_ptr<RigidBody>& rigidBody);
        
        void Init() override;
        void Update(float DeltaTime) override;
        
    private:
        std::shared_ptr<RigidBody> m_rigidBody;
    };
}

