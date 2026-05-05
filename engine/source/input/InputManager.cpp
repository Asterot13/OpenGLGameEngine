#include "InputManager.h"

namespace eng 
{
    void InputManager::SetKeyPressed(int key, bool pressed) 
    {
        if (key < 0 || key >= static_cast<int>(m_keys.size()))
        {
            return;
        }
  
        m_keys[key] = pressed;
    }
    
    bool InputManager::IsKeyPressed(int key) const
    {
        if (key < 0 || key >= static_cast<int>(m_keys.size()))
        {
            return false;
        }
        
        return m_keys[key];
    }

    void InputManager::SetMouseButtonPressed(int button, bool pressed)
    {
        if (button < 0 || button >= static_cast<int>(m_mouseKeys.size()))
        {
            return;
        }
        
        m_mouseKeys[button] = pressed;
    }

    bool InputManager::IsMouseButtonPressed(int button) const
    {
        if (button < 0 || button >= static_cast<int>(m_mouseKeys.size()))
        {
            return false;
        }
        
        return m_mouseKeys[button];
    }
    
    void InputManager::SetMousePositionPrev(const glm::vec2& position)
    {
        m_mousePositionPrev = position;
    }

    const glm::vec2 InputManager::GetMousePositionPrev() const
    {
        return m_mousePositionPrev;
    }

    void InputManager::SetMousePositionCurrent(const glm::vec2& position)
    {
        m_mousePositionCurrent = position;
    }

    const glm::vec2 InputManager::GetMousePositionCurrent() const
    {
        return m_mousePositionCurrent;
    }
}
