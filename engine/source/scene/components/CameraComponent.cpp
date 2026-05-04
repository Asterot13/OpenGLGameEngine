#include "CameraComponent.h"

#include <glm/ext/matrix_clip_space.hpp>
#include "scene/GameObject.h"

namespace eng
{
    void CameraComponent::Update(float DeltaTime)
    {
    }

    glm::mat4 CameraComponent::GetViewMatrix() const
    {
        return glm::inverse(GetOwner()->GetWorldTransform());
    }

    glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
    {
        return glm::perspective(glm::radians(m_fov), aspect, m_nearPlane, m_farPlane);
    }
}
