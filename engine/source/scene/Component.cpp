#include "Component.h"

namespace eng
{
    size_t Component::next_id = 1;
    
    GameObject* Component::GetOwner() const
    {
        return m_owner;
    }
}
