#include "KinematicCharacterController.h"
#include "Engine.h"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace eng
{
    KinematicCharacterController::KinematicCharacterController(float radius, float height, const glm::vec3& position)
        : m_radius(radius), m_height(height)
    {
        auto World = Engine::GetInstance().GetPhysicsManager().GetWorld();
        auto Capsule = new btCapsuleShape(m_radius, m_height);
        m_ghostObject = std::make_unique<btPairCachingGhostObject>();
        
        m_colObjType = CollisionObjectType::KinematicCharacterController;
        
        btTransform start;
        start.setIdentity();
        start.setOrigin(btVector3(position.x, position.y, position.z));
        
        m_ghostObject->setWorldTransform(start);
        m_ghostObject->setCollisionShape(Capsule);
        m_ghostObject->setCollisionFlags(m_ghostObject->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
        m_ghostObject->setUserPointer(this);
        
        World->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
        
        const btScalar stepHeight = 0.35f;
        m_controller = std::make_unique<btKinematicCharacterController>(m_ghostObject.get(), Capsule, stepHeight);
        
        m_controller->setMaxSlope(btRadians(50.f));
        m_controller->setGravity(World->getGravity());
        
        World->addCollisionObject(m_ghostObject.get(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
        World->addAction(m_controller.get());
    }

    KinematicCharacterController::~KinematicCharacterController()
    {
        auto World = Engine::GetInstance().GetPhysicsManager().GetWorld();
        if (m_controller)
        {
            World->removeAction(m_controller.get());
        }
        
        if (m_ghostObject)
        {
            World->removeCollisionObject(m_ghostObject.get());
        }
    }

    glm::vec3 KinematicCharacterController::GetPosition() const
    {
        const auto& position = m_ghostObject->getWorldTransform().getOrigin();
        const glm::vec3 offset(0.0f, m_height + 0.5f + m_radius, 0.0f);
        return glm::vec3(position.x(), position.y(), position.z()) + offset;
    }

    glm::quat KinematicCharacterController::GetRotation() const
    {
        const auto& rotation = m_ghostObject->getWorldTransform().getRotation();
        return glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    }

    void KinematicCharacterController::Walk(const glm::vec3& direction) const
    {
        m_controller->setWalkDirection(btVector3(
            btScalar(direction.x), 
            btScalar(direction.y), 
            btScalar(direction.z)));
    }

    void KinematicCharacterController::Jump(const glm::vec3& direction) const
    {
        if (m_controller->onGround())
        {
            m_controller->jump(btVector3(direction.x, direction.y, direction.z));
        }
    }

    bool KinematicCharacterController::IsGrounded() const
    {
        return m_controller->onGround();
    }
}
