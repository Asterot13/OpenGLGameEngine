#include "PlayerControllerComponent.h"

#include "Engine.h"
#include "scene/GameObject.h"
#include <GLFW/glfw3.h>

namespace eng
{
    void PlayerControllerComponent::Init()
    {
        m_kinematicController = std::make_unique<KinematicCharacterController>(0.4f, 1.2f, m_owner->GetWorldPosition());
    }

    void PlayerControllerComponent::Update(float DeltaTime)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        auto rotation = m_owner->GetRotation();
        
        if (inputManager.IsMousePositionChanged())
        {
            const auto& mousePositionOld = inputManager.GetMousePositionPrev();
            const auto& mousePositionNew = inputManager.GetMousePositionCurrent();
            
            float deltaX = mousePositionNew.x - mousePositionOld.x;
            float deltaY = mousePositionNew.y - mousePositionOld.y;
            
            //Rotation around Y axis
            float yAngle = -deltaX * m_sensitivity * DeltaTime;
            m_yRotation += yAngle;
            glm::quat rotationY = glm::angleAxis(glm::radians(m_yRotation), glm::vec3(0.0f, 1.0f, 0.0f));
            
            //Rotation around X axis
            float xAngle = -deltaY * m_sensitivity * DeltaTime;
            m_xRotation += xAngle;
            m_xRotation = glm::clamp(m_xRotation, -89.0f, 89.0f);
            glm::quat rotationX = glm::angleAxis(glm::radians(m_xRotation), glm::vec3(1.0f, 0.0f, 0.0f));
            
            rotation = glm::normalize(rotationY * rotationX);
            
            m_owner->SetRotation(rotation);
        }
        
        glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        forward.y = 0.0f;
        if (glm::dot(forward, forward) > 0.0f)
        {
            forward = glm::normalize(forward);
        }
        
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        
        glm::vec3 movementDirection(0.0f);
        // Left/Right movement
        if (inputManager.IsKeyPressed(GLFW_KEY_A))
        {
            movementDirection -= right;
        }
        else if (inputManager.IsKeyPressed(GLFW_KEY_D))
        {
            movementDirection += right;
        }
        // Forward/Backward movement
        if (inputManager.IsKeyPressed(GLFW_KEY_W))
        {
            movementDirection += forward;
        }
        else if (inputManager.IsKeyPressed(GLFW_KEY_S))
        {
            movementDirection -= forward;
        }
        
        if (inputManager.IsKeyPressed(GLFW_KEY_SPACE))
        {
            m_kinematicController->Jump(glm::vec3(0.0f, 5.0f, 0.0f));
        }
        
        if (glm::dot(movementDirection, movementDirection) > 0.0f)
        {
            movementDirection = glm::normalize(movementDirection);
        }
        
        m_kinematicController->Walk(movementDirection * m_speed * DeltaTime);
        m_owner->SetPosition(m_kinematicController->GetPosition());
    }
}
