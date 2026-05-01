#pragma once
#include <string>
#include <vector>
#include <memory>
#include "scene/GameObject.h"

namespace eng
{
    class Scene
    {
    public:
        void Update(float DeltaTime);
        void Clear();
        bool SetParent(GameObject* game_object, GameObject* parent);
        GameObject* CreateGameObject(const std::string& name, GameObject* parent = nullptr);

        template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<GameObject, T>>>
        T* CreateGameObject(const std::string& name, GameObject* parent = nullptr)
        {
            auto* obj = new GameObject();
        
            obj->SetName(name);
            SetParent(obj, parent);
            m_gameObjects.push_back(std::unique_ptr<GameObject>(obj));
        
            return static_cast<T*>(obj);
        }
        
    private:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    };
}

