#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "scene/Component.h"

namespace eng
{
    class Scene;
    class GameObject
    {
    public:
        virtual ~GameObject() = default;
        virtual void Init();
        virtual void LoadProperties(const nlohmann::json& properties);
        virtual void Update(float DeltaTime);
        const std::string& GetName() const;
        void SetName(const std::string& name);
        GameObject* GetParent() const;
        GameObject* FindChildByName(const std::string& name);
        bool SetParent(GameObject* parent);
        Scene* GetScene() const;
        
        bool IsAlive() const;
        void MarkForDestroy();
        
        void AddComponent(Component* component);
        
        void SetActive(bool active);
        bool IsActive() const;
        
        template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<Component, T>>>
        T* GetComponent()
        {
            size_t typeID = Component::StaticTypeId<T>();
            for (auto& component : m_components)
            {
                if (component->GetTypeId() == typeID)
                {
                    return static_cast<T*>(component.get());
                }
            }
            return nullptr;
        }
        
        // Transform postion
        const glm::vec3 GetPosition() const;
        glm::vec3 GetWorldPosition() const;
        void SetPosition(const glm::vec3& position);
        void SetWorldPosition(const glm::vec3& position);
        
        // Transform rotation
        const glm::quat GetRotation() const;
        glm::quat GetWorldRotation() const;
        void SetRotation(const glm::quat& rotation);
        void SetWorldRotation(const glm::quat& rotation);
        
        // Transform scale
        const glm::vec3 GetScale() const;
        void SetScale(const glm::vec3& scale);
        
        static GameObject* LoadGLTF(const std::string& path, Scene* gameScene);
        
        glm::mat4 GetLocalTransform() const;
        glm::mat4 GetWorldTransform() const;

    protected:
        GameObject() = default;
        
        std::string m_name;
        GameObject* m_parent = nullptr;
        Scene* m_scene = nullptr;
        std::vector<std::unique_ptr<GameObject>> m_children;
        std::vector<std::unique_ptr<Component>> m_components;
        bool m_isAlive = true;
        bool m_isActive = true;
        
        // Transforms
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 m_scale = glm::vec3(1.0f);
        
        friend class Scene;
    };
    
    class ObjectCreatorBase
    {
    public:
        virtual ~ObjectCreatorBase() = default;
        virtual GameObject* CreateObject() = 0;
    };
    
    template<typename T>
    class ObjectCreator : public ObjectCreatorBase
    {
    public:
        GameObject* CreateObject() override
        {
            return new T();
        }
    };
    
    class GameObjectFactory
    {
    public:
        static GameObjectFactory& GetInstance();
        template<typename T>
        void RegisterObject(const std::string& name)
        {
            m_objectCreators.emplace(name, std::make_unique<ObjectCreator<T>>());
        }
        
        GameObject* CreateGameObject(const std::string& typeName);
        
    private:
        std::unordered_map<std::string, std::unique_ptr<ObjectCreatorBase>> m_objectCreators;
    };
    
#define GAMEOBJECT(ObjectClass) \
public: \
    static void Register() { eng::GameObjectFactory::GetInstance().RegisterObject<ObjectClass>(std::string(#ObjectClass)); }
}
