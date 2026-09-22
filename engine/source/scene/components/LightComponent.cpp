#include "LightComponent.h"

namespace eng
{
    void LightComponent::Update(float DeltaTime)
    {
        
    }

    void LightComponent::LoadProperties(const nlohmann::json& properties)
    {
        if (properties.contains("color"))
        {
            const auto& colorObj = properties["color"];
            glm::vec3 color
            (
                colorObj.value("r", 1.0f),
                colorObj.value("g", 1.0f),
                colorObj.value("b", 1.0f)
            );
            SetColor(color);
        }
    }

    const glm::vec3& LightComponent::GetColor() const
    {
        return m_color;
    }

    void LightComponent::SetColor(const glm::vec3& color)
    {
        m_color = color;
    }
}
