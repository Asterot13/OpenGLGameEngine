#pragma once
#include <memory>
#include <glm/gtc/quaternion.hpp>
#include "Collider.h"

class btRigidBody;
namespace eng
{
    enum class RigidBodyType
    {
        Static,
        Dynamic,
        Kinematic
    };
    
    class RigidBody
    {
    public:
        RigidBody(RigidBodyType type, std::shared_ptr<Collider> collider, float mass, float friction);
        ~RigidBody();
        
        btRigidBody* GetRigidBody();
        void SetAddedToWorld(bool added);
        bool IsAddedToWorld() const;
        
        RigidBodyType GetType() const;
        
        void SetPosition(const glm::vec3& position);
        glm::vec3 GetPosition() const;
        void SetRotation(const glm::quat& rotation);
        glm::quat GetRotation() const;
        
    private:
        std::unique_ptr<btRigidBody> m_rigidBody;
        RigidBodyType m_type = RigidBodyType::Static;
        std::shared_ptr<Collider> m_collider;
        float m_mass = 0.0f;
        float m_friction = 0.5f;
        bool m_addedToWorld = false;
    };
}
