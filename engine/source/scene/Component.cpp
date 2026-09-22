#include "Component.h"

namespace eng
{
    size_t Component::next_id = 1;

    void Component::Init()
    {
    }

    void Component::LoadProperties(const nlohmann::json& properties)
    {
        
    }

    GameObject* Component::GetOwner() const
    {
        return m_owner;
    }
    
    ComponentFactory& ComponentFactory::GetInstance()
    {
        static ComponentFactory instance;
        return instance;
    }
}
