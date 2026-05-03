#include "CameraComponent.h"

#include "scene/GameObject.h"

namespace eng
{
    glm::mat4 CameraComponent::GetViewMatrix() const
    {
        return glm::inverse(GetOwner()->GetWorldTransform());
    }

    glm::mat4 CameraComponent::GetProjectionMatrix() const
    {
        return glm::mat4(1.0f);
    }
}
