#include "JumpPlatform.h"

void JumpPlatform::Init()
{
    auto physics = GetComponent<eng::PhysicsComponent>();
    if (physics)
    {
        auto rigidBody = physics->GetRigidBody();
        if (rigidBody)
        {
            rigidBody->AddContactListener(this);
        }
    }
}

void JumpPlatform::OnContact(eng::CollisionObject* colObj, const glm::vec3& pos, const glm::vec3& normal)
{
    if (colObj->GetCollisionObjectType() == eng::CollisionObjectType::KinematicCharacterController)
    {
        auto controller = static_cast<eng::KinematicCharacterController*>(colObj);
        if (controller)
        {
            controller->Jump(glm::vec3(0.0f, 20.0f, 0.0f));
        }
    }
}
