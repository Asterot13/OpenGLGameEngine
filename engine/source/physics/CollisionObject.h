#pragma once

#include "Common.h"
#include <vector>

namespace eng
{
    class IContactListener;
    
    enum class CollisionObjectType
    {
        Rigidbody,
        KinematicCharacterController,
    };
    
    class CollisionObject
    {
    public:
        CollisionObjectType GetCollisionObjectType() const;
        
        void AddContactListener(IContactListener* listener);
        void RemoveContactListener(IContactListener* listener);
        
    protected:
        void DispatchContactEvent(CollisionObject* colObj, const glm::vec3& pos, const glm::vec3& normal);
        
        CollisionObjectType m_colObjType;
        std::vector<IContactListener*> m_contactListeners;
        
        friend class PhysicsManager;
    };
    
    class IContactListener
    {
        public:
        virtual void OnContact(CollisionObject* colObj, const glm::vec3& pos, const glm::vec3& normal) = 0;
    };
}
