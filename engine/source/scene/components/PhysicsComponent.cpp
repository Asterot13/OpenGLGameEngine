#include "PhysicsComponent.h"

#include "Engine.h"
#include "scene/GameObject.h"

namespace eng
{
    PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& rigidBody)
        : m_rigidBody(rigidBody)
    {
    }

    void PhysicsComponent::Init()
    {
        if (!m_rigidBody)
        {
            return;
        }
        
        const auto position = m_owner->GetWorldPosition();
        const auto rotation = m_owner->GetRotation();
        
        m_rigidBody->SetPosition(position);
        m_rigidBody->SetRotation(rotation);
        
        Engine::GetInstance().GetPhysicsManager().AddRigidBody(m_rigidBody.get());
    }

    void PhysicsComponent::Update(float DeltaTime)
    {
        if (!m_rigidBody)
        {
            return;
        }
        
        if (m_rigidBody->GetType() == RigidBodyType::Dynamic)
        {
            m_owner->SetPosition(m_rigidBody->GetPosition());
            m_owner->SetRotation(m_rigidBody->GetRotation());
        }
    }
}
