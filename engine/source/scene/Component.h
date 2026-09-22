#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace eng
{
    class GameObject;
    class Component
    {
    public:
        virtual ~Component() = default;
        virtual void Init();
        virtual void Update(float DeltaTime) = 0;
        virtual void LoadProperties(const nlohmann::json& properties);
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
    
    class ComponentCreatorBase
    {
    public:
        virtual ~ComponentCreatorBase() = default;
        virtual Component* CreateComponent() = 0;
    };
    
    template<typename T>
    class ComponentCreator : public ComponentCreatorBase
    {
    public:
        Component* CreateComponent() override
        {
            return new T();
        }
    };
    
    // Factory for creating components
    class ComponentFactory
    {
    public:
        static ComponentFactory& GetInstance();
        
        template<typename T>
        void RegisterComponent(const std::string& name)
        {
            m_componentCreators.emplace(name, std::make_unique<ComponentCreator<T>>());
        }
        
        Component* CreateComponent(const std::string& name)
        {
            auto it = m_componentCreators.find(name);
            if (it != m_componentCreators.end())
            {
                return it->second->CreateComponent();
            }
            return nullptr;
        }
        
    private:
        std::unordered_map<std::string, std::unique_ptr<ComponentCreatorBase>> m_componentCreators;
    };
    
#define COMPONENT(ComponentClass) \
public: \
    static size_t TypeId() { return eng::Component::StaticTypeId<ComponentClass>(); } \
    size_t GetTypeId() const override { return TypeId(); } \
    static void Register() { eng::ComponentFactory::GetInstance().RegisterComponent<ComponentClass>(std::string(#ComponentClass)); }
    
}
