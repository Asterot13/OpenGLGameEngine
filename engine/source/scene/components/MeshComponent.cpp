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
}
