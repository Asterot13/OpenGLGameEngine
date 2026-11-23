#include "PhysicsComponent.h"

#include "Engine.h"
#include "scene/GameObject.h"

namespace eng
{
    PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& rigidBody)
        : m_rigidBody(rigidBody)
    {
    }

    void PhysicsComponent::LoadProperties(const nlohmann::json& properties)
    {
        std::shared_ptr<Collider> collider;
        if (properties.contains("collider"))
        {
            const auto& colliderObj = properties["collider"];
            const std::string type = colliderObj.value("type", "");
            
            if (type == "box")
            {
                glm::vec3 extents
                (
                    colliderObj.value("x", 1.0f),
                    colliderObj.value("y", 1.0f),
                    colliderObj.value("z", 1.0f)
                );
                collider = std::make_shared<BoxCollider>(extents);
            }
            else if (type == "sphere")
            {
                float radius = colliderObj.value("r", 1.0f);
                collider = std::make_shared<SphereCollider>(radius);
            }
            else if (type == "capsule")
            {
                float radius = colliderObj.value("r", 1.0f);
                float height = colliderObj.value("h", 1.0f);
                collider = std::make_shared<CapsuleCollider>(radius, height);
            }
            
            if (!collider)
            {
                return;
            }
            
            std::shared_ptr<RigidBody> rigidBody;
            if (properties.contains("body"))
            {
                const auto& bodyObj = properties["body"];
                float mass = bodyObj.value("mass", 0.0f);
                float friction = bodyObj.value("friction", 0.0f);
                std::string strType = bodyObj.value("type", "static");
                
                RigidBodyType BodyType = RigidBodyType::Static;
                if (strType == "dynamic")
                {
                    BodyType = RigidBodyType::Dynamic;
                }
                else if (strType == "kinematic")
                {
                    BodyType = RigidBodyType::Kinematic;
                }
                
                rigidBody = std::make_shared<RigidBody>(BodyType, collider, mass, friction);
            }
            
            if (rigidBody)
            {
                SetRigidBody(rigidBody);
            }
        }
    }

    void PhysicsComponent::Init()
    {
        if (!m_rigidBody)
        {
            return;
        }
        
        const auto position = m_owner->GetWorldPosition();
        const auto rotation = m_owner->GetWorldRotation();
        
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
            m_owner->SetWorldPosition(m_rigidBody->GetPosition());
            m_owner->SetWorldRotation(m_rigidBody->GetRotation());
        }
    }

    void PhysicsComponent::SetRigidBody(const std::shared_ptr<RigidBody>& rigidBody)
    {
        m_rigidBody = rigidBody;
    }

    const std::shared_ptr<RigidBody>& PhysicsComponent::GetRigidBody()
    {
        return m_rigidBody;
    }
}
