#include "CollisionObject.h"

namespace eng
{
    CollisionObjectType CollisionObject::GetCollisionObjectType() const
    {
        return m_colObjType;
    }

    void CollisionObject::AddContactListener(IContactListener* listener)
    {
        m_contactListeners.push_back(listener);
    }

    void CollisionObject::RemoveContactListener(IContactListener* listener)
    {
        auto it = std::find(m_contactListeners.begin(), m_contactListeners.end(), listener);
        if (it != m_contactListeners.end())
        {
            m_contactListeners.erase(it);
        }
    }

    void CollisionObject::DispatchContactEvent(CollisionObject* colObj, const glm::vec3& pos, const glm::vec3& normal)
    {
        for (auto& listener : m_contactListeners)
        {
            listener->OnContact(colObj, pos, normal);
        }
    }
}
