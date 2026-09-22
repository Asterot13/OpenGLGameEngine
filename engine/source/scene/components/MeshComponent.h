#pragma once
#include <memory>
#include "scene/Component.h"

namespace eng
{
    class Material;
    class Mesh;
    
    class MeshComponent : public Component
    {
        COMPONENT(MeshComponent)
    public:
        MeshComponent() = default;
        MeshComponent(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
        void Update(float DeltaTime) override;
        
        void LoadProperties(const nlohmann::json& properties) override;
        
        void SetMesh(const std::shared_ptr<Mesh>& mesh);
        void SetMaterial(const std::shared_ptr<Material>& material);
        
    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<Material> m_material;
    };
}
