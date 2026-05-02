#include "Scene.h"

#include <algorithm>

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
        SetParent(obj, parent);
        
        return obj;
    }
}
