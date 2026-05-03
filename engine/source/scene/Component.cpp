#include "Component.h"

namespace eng
{
    GameObject* Component::GetOwner() const
    {
        return m_owner;
    }
}
