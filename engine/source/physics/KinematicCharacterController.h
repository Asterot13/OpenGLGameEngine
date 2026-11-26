#pragma once
#include <memory>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include "CollisionObject.h"

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace eng
{
    class KinematicCharacterController : public CollisionObject
    {
    public:
        KinematicCharacterController(float radius, float height, const glm::vec3& position);
        ~KinematicCharacterController();
        
        glm::vec3 GetPosition() const;
        glm::quat GetRotation() const;
        
        void Walk(const glm::vec3& direction) const;
        void Jump(const glm::vec3& direction) const;
        bool IsGrounded() const;
        
    private:
        float m_height = 1.2f;
        float m_radius = 0.4f;
        
        std::unique_ptr<btPairCachingGhostObject> m_ghostObject;
        std::unique_ptr<btKinematicCharacterController> m_controller;
    };
}
