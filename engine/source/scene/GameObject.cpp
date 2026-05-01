#include "GameObject.h"

namespace eng
{
    void GameObject::Update(float DeltaTime)
    {
        for (auto it = m_children.begin(); it != m_children.end(); ++it)
        {
            if ((*it)->IsAlive())
            {
                (*it)->Update(DeltaTime);
                ++it;
            }
            else
            {
                m_children.erase(it);
            }           
        }
    }

    const std::string& GameObject::GetName() const
    {
        return m_name;
    }

    void GameObject::SetName(const std::string& name)
    {
        m_name = name;
    }

    GameObject* GameObject::GetParent() const
    {
        return m_parent;
    }

    bool GameObject::IsAlive() const
    {
        return m_isAlive;
    }

    void GameObject::MarkForDestroy()
    {
        m_isAlive = false;
    }
}
