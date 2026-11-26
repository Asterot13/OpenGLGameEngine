#include "MeshComponent.h"
#include <iostream>
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
            auto& materialObj = properties["material"];
            const std::string path = materialObj.value("path", "");
            auto material = Material::Load(path);
            if (material && materialObj.contains("params"))
            {
                auto& paramsObj = materialObj["params"];
            
                //floats
                if (paramsObj.contains("float"))
                {
                    for (auto& floatParam : paramsObj["float"])
                    {
                        std::string name = floatParam.value("name", "");
                        float value = floatParam.value("value", 0.0f);
                        material->SetParam(name, value); 
                    }
                }
                
                //float2
                if (paramsObj.contains("float2"))
                {
                    for (auto& floatParam : paramsObj["float2"])
                    {
                        std::string name = floatParam.value("name", "");
                        float value0 = floatParam.value("value0", 0.0f);
                        float value1 = floatParam.value("value1", 0.0f);
                        material->SetParam(name, value0, value1);
                    }
                }
                
                //float3
                if (paramsObj.contains("float3"))
                {
                    for (auto& floatParam : paramsObj["float3"])
                    {
                        std::string name = floatParam.value("name", "");
                        float value0 = floatParam.value("value0", 0.0f);
                        float value1 = floatParam.value("value1", 0.0f);
                        float value2 = floatParam.value("value2", 0.0f);
                        material->SetParam(name, glm::vec3(value0, value1, value2));
                    }
                }
                
                //textures
                if (paramsObj.contains("textures"))
                {
                    for (auto& textureParam : paramsObj["textures"])
                    {
                        std::string name = textureParam.value("name", "");
                        std::string texturePath = textureParam.value("path", textureParam.value("value", ""));
                        auto texture = Texture::LoadFromFile(texturePath);
                        if (!texture)
                        {
                            std::cerr << "Failed to load texture '" << texturePath << "' for material '" << path << "'\n";
                            continue;
                        }
                        
                        material->SetParam(name, texture); 
                    }
                }
            }
            SetMaterial(material);
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
