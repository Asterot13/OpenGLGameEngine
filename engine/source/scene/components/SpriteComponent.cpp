#include "SpriteComponent.h"

#include "Engine.h"
#include "graphics/Texture.h"
#include "render/RenderQueue.h"
#include "scene/GameObject.h"

namespace eng
{
    void SpriteComponent::Update(float DeltaTime)
    {
        if (!m_texture || !m_visible)
        {
            return;
        }
        
        RenderCommand2D command;
        command.modelMatrix = GetOwner()->GetWorldTransform2D();
        command.texture = m_texture.get();
        command.color = m_color;
        command.size = m_size;
        command.lowerLeftUV = m_lowerLeftUV;
        command.upperRightUV = m_upperRightUV;
        command.pivot = m_pivot;
        
        auto& renderQueue = Engine::GetInstance().GetRenderQueue();
        renderQueue.Submit(command);
    }

    void SpriteComponent::SetTexture(const std::shared_ptr<Texture>& texture)
    {
        m_texture = texture;
    }

    const std::shared_ptr<Texture>& SpriteComponent::GetTexture() const
    {
        return m_texture;
    }

    void SpriteComponent::SetColor(const glm::vec4& color)
    {
        m_color = color;
    }

    const glm::vec4& SpriteComponent::GetColor() const
    {
        return m_color;
    }

    void SpriteComponent::SetSize(const glm::vec2& size)
    {
        m_size = size;
    }

    const glm::vec2& SpriteComponent::GetSize() const
    {
        return m_size;
    }

    void SpriteComponent::SetLowerLeftUV(const glm::vec2& lowerLeftUV)
    {
        m_lowerLeftUV = lowerLeftUV;
    }

    const glm::vec2& SpriteComponent::GetLowerLeftUV() const
    {
        return m_lowerLeftUV;
    }

    void SpriteComponent::SetUpperRightUV(const glm::vec2& upperRightUV)
    {
        m_upperRightUV = upperRightUV;
    }

    const glm::vec2& SpriteComponent::GetUpperRightUV() const
    {
        return m_upperRightUV;
    }

    void SpriteComponent::SetUV(const glm::vec2& lowerLeftUV, const glm::vec2& upperRightUV)
    {
        m_lowerLeftUV = lowerLeftUV;
        m_upperRightUV = upperRightUV;
    }

    void SpriteComponent::SetPivot(const glm::vec2& pivot)
    {
        m_pivot = pivot;
    }

    const glm::vec2& SpriteComponent::GetPivot() const
    {
        return m_pivot;
    }

    void SpriteComponent::SetVisible(bool visible)
    {
        m_visible = visible;
    }

    bool SpriteComponent::GetVisible() const
    {
        return m_visible;
    }

    void SpriteComponent::LoadProperties(const nlohmann::json& properties)
    {
        // Texture
        const std::string texturePath = properties.value("texture", "");
        if (auto texture = Texture::LoadFromFile(texturePath))
        {
            SetTexture(texture);
        }
        
        // Color
        if (properties.contains("color"))
        {
            auto& colorObj = properties["color"];
            glm::vec4 color(
                colorObj.value("r", 1.0f),
                colorObj.value("g", 1.0f),
                colorObj.value("b", 1.0f),
                colorObj.value("a", 1.0f)
                );
            
            SetColor(color);
        }
        
        // Size
        if (properties.contains("size"))
        {
            auto& sizeObj = properties["size"];
            glm::vec2 size(
                sizeObj.value("x", 100.0f),
                sizeObj.value("y", 100.0f)
                );
            
            SetSize(size);
        }
        
        // Lower Left UV
        if (properties.contains("lowerLeftUV"))
        {
            auto& leftUVObj = properties["lowerLeftUV"];
            glm::vec2 leftUV(
                leftUVObj.value("u", 0.0f),
                leftUVObj.value("v", 0.0f)
                );
            
            SetLowerLeftUV(leftUV);
        }
        
        // Upper Right UV
        if (properties.contains("upperRightUV"))
        {
            auto& rightUVObj = properties["upperRightUV"];
            glm::vec2 righttUV(
                rightUVObj.value("u", 1.0f),
                rightUVObj.value("v", 1.0f)
                );
            
            SetUpperRightUV(righttUV);
        }
        
        // Pivot
        if (properties.contains("pivot"))
        {
            auto& pivotObj = properties["pivot"];
            glm::vec2 pivot(
                pivotObj.value("x", 0.5f),
                pivotObj.value("y", 0.5f)
                );
            
            SetPivot(pivot);
        }
    }
}
