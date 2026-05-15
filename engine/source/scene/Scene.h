#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Common.h"
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
        
        //Camera
        void SetMainCamera(GameObject* camera);
        GameObject* GetMainCamera() const;
        
        std::vector<LightData> CollectLights();

        template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<GameObject, T>>>
        T* CreateGameObject(const std::string& name, GameObject* parent = nullptr)
        {
            auto* obj = new T();
        
            obj->SetName(name);
            SetParent(obj, parent);
        
            return obj;
        }
        
    private:
        void CollectLightsRecursive(GameObject* obj, std::vector<LightData>& lights);
        
    private:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
        GameObject* m_mainCamera = nullptr;
    };
}

