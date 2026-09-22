#include "MeshComponent.h"
#include "Engine.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include "render/RenderQueue.h"
#include "scene/GameObject.h"

namespace eng
{
    MeshComponent::MeshComponent(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) :
        m_mesh(mesh), m_material(material) 
    {
    }

    void MeshComponent::Update(float DeltaTime)
    {
        if (!m_material || !m_mesh)
        {
            return;
        }
        
        RenderCommand command;
        command.material = m_material.get();
        command.mesh = m_mesh.get();
        command.modelMatrix = GetOwner()->GetWorldTransform();
        
        Engine::GetInstance().GetRenderQueue().Submit(command);
    }

    void MeshComponent::LoadProperties(const nlohmann::json& properties)
    {
        if (properties.contains("material"))
        {
            const std::string& materialPath = properties.value("material", "");
            auto material = Material::Load(materialPath);
            if (material)
            {
                SetMaterial(material);
            }
        }
        
        if (properties.contains("mesh"))
        {
            const auto& meshObj = properties["mesh"];
            const std::string type = meshObj.value("type", "box");
            if (type == "box")
            {
                glm::vec3 extents = glm::vec3(
                    meshObj.value("x", 1.0f),
                    meshObj.value("y", 1.0f),
                    meshObj.value("z", 1.0f)
                );
                auto mesh = Mesh::CreateBox(extents);
                SetMesh(mesh);
            }
        }
    }

    void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh)
    {
        m_mesh = mesh;
    }

    void MeshComponent::SetMaterial(const std::shared_ptr<Material>& material)
    {
        m_material = material;
    }
}
