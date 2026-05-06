#include "PlayerControllerComponent.h"

#include "Engine.h"
#include "scene/GameObject.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

namespace eng
{
    void PlayerControllerComponent::Update(float DeltaTime)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        auto rotation = m_owner->GetRotation();
        
        if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            const auto& mousePositionOld = inputManager.GetMousePositionPrev();
            const auto& mousePositionNew = inputManager.GetMousePositionCurrent();
            
            float deltaX = mousePositionNew.x - mousePositionOld.x;
            float deltaY = mousePositionNew.y - mousePositionOld.y;
            
            //Rotation around Y axis
            float yAngle = -deltaX * m_sensitivity * DeltaTime;
            glm::quat rotationY = glm::angleAxis(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            
            //Rotation around X axis
            float xAngle = -deltaY * m_sensitivity * DeltaTime;
            glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
            glm::quat rotationX = glm::angleAxis(xAngle, right);
            
            glm::quat deltaRotation = rotationX * rotationY;
            rotation = glm::normalize(deltaRotation * rotation);
            
            m_owner->SetRotation(rotation);
        }
        
        glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
        
        auto position = m_owner->GetPosition();
        
        // Left/Right movement
        if (inputManager.IsKeyPressed(GLFW_KEY_A))
        {
            position -= right * m_speed * DeltaTime;
        }
        else if (inputManager.IsKeyPressed(GLFW_KEY_D))
        {
            position += right * m_speed * DeltaTime;
        }
        // Forward/Backward movement
        if (inputManager.IsKeyPressed(GLFW_KEY_W))
        {
            position += forward * m_speed * DeltaTime;
        }
        else if (inputManager.IsKeyPressed(GLFW_KEY_S))
        {
            position -= forward * m_speed * DeltaTime;
        }
        m_owner->SetPosition(position);
        
    }
}
