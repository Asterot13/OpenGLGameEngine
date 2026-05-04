#pragma once

namespace eng
{
    class GameObject;
    class Component
    {
    public:
        virtual ~Component() = default;
        virtual void Update(float DeltaTime) = 0;
        virtual size_t GetTypeId() const = 0;
        
        GameObject* GetOwner() const;
        
        template<typename T>
        static size_t StaticTypeId()
        {
            static size_t typeID = next_id++;
            return typeID;
        }
        
    protected:
        GameObject* m_owner = nullptr;
        
        friend class GameObject;
        
    private:
        static size_t next_id;
    };
    
#define COMPONENT(ComponentClass) \
public: \
    static size_t TypeId() { return Component::StaticTypeId<ComponentClass>(); } \
    size_t GetTypeId() const override { return TypeId(); }
    
}
