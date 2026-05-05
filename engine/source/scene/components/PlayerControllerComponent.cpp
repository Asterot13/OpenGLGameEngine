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
            
            rotation.y -= deltaX * m_sensitivity * DeltaTime;
            rotation.x += deltaY * m_sensitivity * DeltaTime;
            
            m_owner->SetRotation(rotation);
        }
        
        glm::mat4 rotationMatrix(1.0f);

        rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X - axis
        rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y - axis
        rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z - axis
        
        glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        glm::vec3 right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
        
        auto position = m_owner->GetPosition();
        
        // Left/Right movement
        if (inputManager.IsKeyPressed(GLFW_KEY_A))
        {
            position += right * m_speed * DeltaTime;
        }
        else if (inputManager.IsKeyPressed(GLFW_KEY_D))
        {
            position -= right * m_speed * DeltaTime;
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
