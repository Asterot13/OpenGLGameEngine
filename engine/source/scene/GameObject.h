#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "scene/Component.h"

namespace eng
{
    class GameObject
    {
    public:
        virtual ~GameObject() = default;
        virtual void Update(float DeltaTime);
        const std::string& GetName() const;
        void SetName(const std::string& name);
        GameObject* GetParent() const;
        bool IsAlive() const;
        void MarkForDestroy();
        
        void AddComponent(Component* component);
        
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
        void SetPosition(const glm::vec3& position);
        // Transform rotation
        const glm::vec3 GetRotation() const;
        void SetRotation(const glm::vec3& position);
        // Transform scale
        const glm::vec3 GetScale() const;
        void SetScale(const glm::vec3& scale);
        
        glm::mat4 GetLocalTransform() const;
        glm::mat4 GetWorldTransform() const;
        
    protected:
        GameObject() = default;
        
    private:
        std::string m_name;
        GameObject* m_parent = nullptr;
        std::vector<std::unique_ptr<GameObject>> m_children;
        std::vector<std::unique_ptr<Component>> m_components;
        bool m_isAlive = true;
        
        // Transforms
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_rotation = glm::vec3(0.0f);
        glm::vec3 m_scale = glm::vec3(1.0f);
        
        friend class Scene;
    };
}
