#include "RigidBody.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <utility>
#include "Engine.h"

namespace eng
{
    RigidBody::RigidBody(RigidBodyType type, std::shared_ptr<Collider> collider, float mass, float friction)
        : m_type(type), m_collider(std::move(collider)), m_mass(mass), m_friction(friction)
    {
        if (!m_collider)
        {
            return;
        }
        
        btVector3 inertia(0, 0, 0);
        if (m_type == RigidBodyType::Dynamic && mass > 0.0f && m_collider->GetShape())
        {
            m_collider->GetShape()->calculateLocalInertia(btScalar(mass), inertia);
        }
        
        btTransform transform;
        transform.setIdentity();
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);
        
        btRigidBody::btRigidBodyConstructionInfo rbInfo(
            (m_type == RigidBodyType::Dynamic) ? mass : 0.0f,
            motionState,
            m_collider->GetShape(),
            inertia
        );
        
        m_rigidBody = std::make_unique<btRigidBody>(rbInfo);
        m_rigidBody->setFriction(m_friction);
        
        if (m_type == RigidBodyType::Kinematic)
        {
            m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
            m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
        }
    }

    RigidBody::~RigidBody()
    {
        if (m_addedToWorld)
        {
            Engine::GetInstance().GetPhysicsManager().RemoveRigidBody(this);
        }
    }

    btRigidBody* RigidBody::GetRigidBody()
    {
        return m_rigidBody.get();
    }

    void RigidBody::SetAddedToWorld(bool added)
    {
        m_addedToWorld = added;
    }

    bool RigidBody::IsAddedToWorld() const
    {
        return m_addedToWorld;
    }

    RigidBodyType RigidBody::GetType() const
    {
        return m_type;
    }

    void RigidBody::SetPosition(const glm::vec3& position)
    {
        if (!m_rigidBody)
        {
            return;
        }
        
        auto& transform = m_rigidBody->getWorldTransform();
        transform.setOrigin(btVector3(position.x, position.y, position.z));
        if (m_rigidBody->getMotionState())
        {
            m_rigidBody->getMotionState()->setWorldTransform(transform);
        }
        m_rigidBody->setWorldTransform(transform);
    }

    glm::vec3 RigidBody::GetPosition() const
    {
        const auto& position = m_rigidBody->getWorldTransform().getOrigin();
        return glm::vec3(position.x(), position.y(), position.z());
    }

    void RigidBody::SetRotation(const glm::quat& rotation)
    {
        if (!m_rigidBody)
        {
            return;
        }
        
        auto& transform = m_rigidBody->getWorldTransform();
        transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
        if (m_rigidBody->getMotionState())
        {
            m_rigidBody->getMotionState()->setWorldTransform(transform);
        }
        m_rigidBody->setWorldTransform(transform);
    }

    glm::quat RigidBody::GetRotation() const
    {
        const auto& rotation = m_rigidBody->getWorldTransform().getRotation();
        return glm::quat(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    }
}
