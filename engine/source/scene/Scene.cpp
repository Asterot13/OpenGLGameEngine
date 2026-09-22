#include "Scene.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include "Engine.h"
#include "components/AnimationComponent.h"
#include "components/MeshComponent.h"
#include "components/LightComponent.h"
#include "components/CameraComponent.h"
#include "components/PhysicsComponent.h"
#include "components/PlayerControllerComponent.h"

namespace eng
{
    void Scene::Update(float DeltaTime)
    {
        for (auto it = m_gameObjects.begin(); it != m_gameObjects.end();)
        {
            if ((*it)->IsAlive())
            {
                (*it)->Update(DeltaTime);
                ++it;
            }
            else
            {
                it = m_gameObjects.erase(it);
            }           
        }
    }
    
    void Scene::Clear()
    {
        m_gameObjects.clear();
    }

    bool Scene::SetParent(GameObject* game_object, GameObject* parent)
    {
        bool result = false;
        auto CurrParent = game_object->GetParent();
        if (parent == nullptr)
        {
            if (CurrParent != nullptr)
            {
                auto it = std::find_if(
                    CurrParent->m_children.begin(), 
                    CurrParent->m_children.end(), 
                    [game_object](const std::unique_ptr<GameObject>& el)
                    {
                        return el.get() == game_object;
                    });
                
                if (it != CurrParent->m_children.end())
                {
                    m_gameObjects.push_back(std::move(*it));
                    game_object->m_parent = nullptr;
                    CurrParent->m_children.erase(it);
                    result = true;
                }
            }
            // No parent currently. This can be in two cases
            // 1. The object is in the scene root
            // 2. The object has been just created
            else
            {
                auto it = std::find_if(
                    m_gameObjects.begin(), 
                    m_gameObjects.end(), 
                    [game_object](const std::unique_ptr<GameObject>& el)
                    {
                        return el.get() == game_object;
                    });
                
                // The object has been just created
                if (it == m_gameObjects.end())
                {
                    std::unique_ptr<GameObject> objHolder(game_object);
                    m_gameObjects.push_back(std::move(objHolder));
                    result = true;
                }               
            }
        }
        // We are trying to add it as a child of another object
        else
        {
            if (CurrParent != nullptr)
            {
                auto it = std::find_if(
                    CurrParent->m_children.begin(), 
                    CurrParent->m_children.end(), 
                    [game_object](const std::unique_ptr<GameObject>& el)
                    {
                        return el.get() == game_object;
                    });
                
                if (it != CurrParent->m_children.end())
                {
                    bool foundCircularParent = false;
                    auto currElement = parent;
                    while (currElement != nullptr)
                    {
                        if (currElement == game_object)
                        {
                            foundCircularParent = true;
                            break;
                        }
                        currElement = currElement->GetParent();
                    }
                    
                    if (!foundCircularParent)
                    {
                        parent->m_children.push_back(std::move(*it));
                        game_object->m_parent = parent;
                        CurrParent->m_children.erase(it);
                        result = true;
                        
                    }                   
                }
            }
            // No parent currently. This can be in two cases
            // 1. The object is in the scene root
            // 2. The object has been just created
            else
            {
                auto it = std::find_if(
                    m_gameObjects.begin(), 
                    m_gameObjects.end(), 
                    [game_object](const std::unique_ptr<GameObject>& el)
                    {
                        return el.get() == game_object;
                    });
                
                // The object has been just created
                if (it == m_gameObjects.end())
                {
                    std::unique_ptr<GameObject> objHolder(game_object);
                    parent->m_children.push_back(std::move(objHolder));
                    game_object->m_parent = parent;
                    result = true;
                }
                // The object is in the scene root
                else
                {
                    bool foundCircularParent = false;
                    auto currElement = parent;
                    while (currElement != nullptr)
                    {
                        if (currElement == game_object)
                        {
                            foundCircularParent = true;
                            break;
                        }
                        currElement = currElement->GetParent();
                    }
                    
                    if (!foundCircularParent)
                    {
                        parent->m_children.push_back(std::move(*it));
                        game_object->m_parent = parent;
                        m_gameObjects.erase(it);
                        result = true;
                    }                   
                }
            }
        }
        return result;
    }

    GameObject* Scene::CreateGameObject(const std::string& name, GameObject* parent)
    {
        auto* obj = new GameObject();
        
        obj->SetName(name);
        obj->m_scene = this;
        SetParent(obj, parent);
        
        return obj;
    }

    GameObject* Scene::CreateGameObject(const std::string& type, const std::string& name, GameObject* parent)
    {
        auto* obj = GameObjectFactory::GetInstance().CreateGameObject(type);
        if (obj)
        {
            obj->SetName(name);
            obj->m_scene = this;
            SetParent(obj, parent);
        }
        
        return obj;
    }

    void Scene::RegisterTypes()
    {
        AnimationComponent::Register();
        CameraComponent::Register();
        LightComponent::Register();
        MeshComponent::Register();
        PhysicsComponent::Register();
        PlayerControllerComponent::Register();
    }

    void Scene::SetMainCamera(GameObject* camera)
    {
        m_mainCamera = camera;
    }

    GameObject* Scene::GetMainCamera() const
    {
        return m_mainCamera;
    }

    std::vector<LightData> Scene::CollectLights()
    {
        std::vector<LightData> lights;
        for (auto& obj : m_gameObjects)
        {
            CollectLightsRecursive(obj.get(), lights);
        }
        return lights;       
    }

    std::shared_ptr<Scene> Scene::Load(const std::string& path)
    {
        const std::string& data = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
        if (data.empty())
        {
            return nullptr;
        }
        
        auto json = nlohmann::json::parse(data);
        if (json.empty())
        {
            return nullptr;
        }
        
        auto result = std::make_shared<Scene>();
        const std::string& sceneName = json.value("name", "noname");
        if (json.contains("objects") && json["objects"].is_array())
        {
            const auto& objects = json["objects"];
            for (const auto& obj : objects)
            {
                result->LoadObject(obj, nullptr);
            }
        }
        
        if (json.contains("camera"))
        {
            std::string cameraObjName = json.value("camera", "");
            for (const auto& child : result->m_gameObjects)
            {
                if (auto object = child->FindChildByName(cameraObjName))
                {
                    result->SetMainCamera(object);
                    break;
                }
            }
        }
        
        return result;
    }

    void Scene::CollectLightsRecursive(GameObject* obj, std::vector<LightData>& lights)
    {
        if (auto light = obj->GetComponent<LightComponent>())
        {
            LightData lightData;
            lightData.position = obj->GetWorldPosition();
            lightData.color = light->GetColor();
            lights.push_back(lightData);
        }
        
        for (auto& child : obj->m_children)
        {
            CollectLightsRecursive(child.get(), lights);
        }       
    }

    void Scene::LoadObject(const nlohmann::json& json, GameObject* parent)
    {
        const std::string& name = json.value("name", "Object");
        GameObject* gameObject = nullptr;
        
        if (json.contains("type"))
        {
            const std::string& type = json.value("type", "");
            if (type == "gltf")
            {
                std::string path = json.value("path", "");
                gameObject = GameObject::LoadGLTF(path, this);
                if (gameObject)
                {
                    gameObject->SetName(name);
                    gameObject->SetParent(parent);
                    
                }               
            }
            else
            {
                gameObject = CreateGameObject(type, name, parent);
            }
        }
        else
        {
            gameObject = CreateGameObject(name, parent);
        }
        
        if (!gameObject)
        {
            return;
        }
        
        if (json.contains("position"))
        {
            auto posObj = json["position"];
            glm::vec3 position;
            position.x = posObj.value("x", 0.0f);
            position.y = posObj.value("y", 0.0f);
            position.z = posObj.value("z", 0.0f);
            gameObject->SetPosition(position);
        }
        
        if (json.contains("rotation"))
        {
            auto rotObj = json["rotation"];
            glm::quat rotation;
            rotation.x = rotObj.value("x", 0.0f);
            rotation.y = rotObj.value("y", 0.0f);
            rotation.z = rotObj.value("z", 0.0f);
            rotation.w = rotObj.value("w", 1.0f);
            gameObject->SetRotation(rotation);
        }
        
        if (json.contains("scale"))
        {
            auto scaleObj = json["scale"];
            glm::vec3 scale;
            scale.x = scaleObj.value("x", 1.0f);
            scale.y = scaleObj.value("y", 1.0f);
            scale.z = scaleObj.value("z", 1.0f);
            gameObject->SetScale(scale);
        }
        
        gameObject->LoadProperties(json);
        
        if (json.contains("components") && json["components"].is_array())
        {
            const auto& components = json["components"];
            for (const auto& component : components)
            {
                const std::string& type = component.value("type", "");
                Component* newComponent = ComponentFactory::GetInstance().CreateComponent(type);
                if (newComponent)
                {
                    newComponent->LoadProperties(component);
                    gameObject->AddComponent(newComponent);
                }
            }       
        }
        
        if (json.contains("children") && json["children"].is_array())
        {
            const auto& children = json["children"];
            for (const auto& child : children)
            {
                LoadObject(child, gameObject);
            }      
        }
        
        gameObject->Init();
    }
}
