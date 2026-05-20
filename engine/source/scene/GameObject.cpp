#include "GameObject.h"
#include <cgltf.h>
#include "Engine.h"
#include "Scene.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "graphics/Texture.h"
#include "graphics/VertexLayout.h"
#include "components/MeshComponent.h"
#include "components/AnimationComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>



namespace eng
{
    void GameObject::Update(float DeltaTime)
    {
        if (!m_isActive)
        {
            return;
        }
        
        // Update components logic
        for (auto& component : m_components)
        {
            component->Update(DeltaTime);
        }
        // Update children logic
        for (auto it = m_children.begin(); it != m_children.end();)
        {
            if ((*it)->IsAlive())
            {
                (*it)->Update(DeltaTime);
                ++it;
            }
            else
            {
                it = m_children.erase(it);
            }           
        }
    }

    const std::string& GameObject::GetName() const
    {
        return m_name;
    }

    void GameObject::SetName(const std::string& name)
    {
        m_name = name;
    }

    GameObject* GameObject::GetParent() const
    {
        return m_parent;
    }

    GameObject* GameObject::FindChildByName(const std::string& name)
    {
        if (m_name == name)
        {
            return this;
        }
        
        for (auto& child : m_children)
        {
            if (auto result = child->FindChildByName(name))
            {
                return result;
            }
        }
        
        return nullptr;
    }

    bool GameObject::SetParent(GameObject* parent)
    {
        if (!m_scene)
        {
            return false;
        }
        
        return m_scene->SetParent(this, parent);
    }

    Scene* GameObject::GetScene() const
    {
        return m_scene;
    }

    bool GameObject::IsAlive() const
    {
        return m_isAlive;
    }

    void GameObject::MarkForDestroy()
    {
        m_isAlive = false;
    }

    void GameObject::AddComponent(Component* component)
    {
        m_components.emplace_back(component);
        component->m_owner = this;
    }

    void GameObject::SetActive(bool active)
    {
        m_isActive = active;
    }

    bool GameObject::IsActive() const
    {
        return m_isActive;
    }

    const glm::vec3 GameObject::GetPosition() const
    {
        return m_position;
    }

    glm::vec3 GameObject::GetWorldPosition() const
    {
        glm::vec4 hom = GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        return glm::vec3(hom) / hom.w;
    }

    void GameObject::SetPosition(const glm::vec3& position)
    {
        m_position = position;
    }

    const glm::quat GameObject::GetRotation() const
    {
        return m_rotation;      
    }

    void GameObject::SetRotation(const glm::quat& rotation)
    {
        m_rotation = rotation;      
    }

    const glm::vec3 GameObject::GetScale() const
    {
        return m_scale;     
    }

    void GameObject::SetScale(const glm::vec3& scale)
    {
        m_scale = scale;     
    }
    
    void ParseGLTFNode(const cgltf_node* node, GameObject* parent, const std::filesystem::path& relativeFolderPath)
    {
        auto object = parent->GetScene()->CreateGameObject(node->name, parent);
        
        // Parsing object params
        if (node->has_matrix)
        {
            auto mat = glm::make_mat4(node->matrix);
            glm::vec3 translation, scale, skew;
            glm::vec4 perspective;
            glm::quat orientation;
            glm::decompose(mat, scale, orientation, translation, skew, perspective);
            
            object->SetPosition(translation);
            object->SetRotation(orientation);
            object->SetScale(scale);
        }
        else
        {
            if (node->has_translation)
            {
                object->SetPosition(glm::vec3(node->translation[0], node->translation[1], node->translation[2]));
            }
            
            if (node->has_rotation)
            {
                object->SetRotation(glm::quat(node->rotation[3], node->rotation[0], node->rotation[1], node->rotation[2]));
            }
            
            if (node->has_scale)
            {
                object->SetScale(glm::vec3(node->scale[0], node->scale[1], node->scale[2]));
            }
        }
        
        // Parsing mesh
        if (node->mesh)
        {
            for (cgltf_size pi = 0; pi < node->mesh->primitives_count; ++pi)
            {
                auto& primitive = node->mesh->primitives[pi];
                if (primitive.type != cgltf_primitive_type_triangles)
                {
                    continue;
                }
                
                auto readFloats = [](const cgltf_accessor* acc, cgltf_size i, float* out, int n)
                {
                    std::fill(out, out + n, 0.0f);
                    return cgltf_accessor_read_float(acc, i, out, n) == 1;
                };
        
                auto readIndices = [](const cgltf_accessor* acc, cgltf_size i)
                {
                    cgltf_uint out = 0;
                    cgltf_bool success = cgltf_accessor_read_uint(acc, i, &out, 1);
                    return success ? static_cast<uint32_t>(out) : 0;
                };
                
                VertexLayout layout;
                cgltf_accessor* accessor[4] = {nullptr, nullptr, nullptr, nullptr};
                
                for (cgltf_size attIndex = 0; attIndex < primitive.attributes_count; ++attIndex)
                {
                    auto& attribute = primitive.attributes[attIndex];
                    auto acc = attribute.data;
                    if (!acc)
                    {
                        continue;
                    }
                    
                    VertexElement element;
                    element.type = GL_FLOAT;
                    
                    switch (attribute.type)
                    {
                        case cgltf_attribute_type_position:
                            {
                                accessor[VertexElement::PositionIndex] = acc;
                                element.index = VertexElement::PositionIndex;
                                element.size = 3;
                                break;
                            }
                        case cgltf_attribute_type_color:
                            {
                                if (attribute.index != 0)
                                {
                                    continue;
                                }
                                accessor[VertexElement::ColorIndex] = acc;
                                element.index = VertexElement::ColorIndex;
                                element.size = 3;
                                break;
                            }
                        case cgltf_attribute_type_texcoord:
                            {
                                if (attribute.index != 0)
                                {
                                    continue;
                                }
                                accessor[VertexElement::UVIndex] = acc;
                                element.index = VertexElement::UVIndex;
                                element.size = 2;
                                break;
                            }
                        case cgltf_attribute_type_normal:
                            {
                                accessor[VertexElement::NormalIndex] = acc;
                                element.index = VertexElement::NormalIndex;
                                element.size = 3;
                                break;
                            }
                        default:
                            continue;
                    }
                    
                    if (element.size > 0)
                    {
                        element.offset = layout.stride;
                        layout.stride += element.size * sizeof(float);
                        layout.elements.push_back(element);
                    }
                }
                
                if (!accessor[VertexElement::PositionIndex])
                {
                    continue;
                }
                
                auto vertexCount = accessor[VertexElement::PositionIndex]->count;
                std::vector<float> vertices;
                vertices.resize(layout.stride / sizeof(float) * vertexCount);
                for (cgltf_size vi = 0; vi < vertexCount; ++vi)
                {
                    for (auto& element : layout.elements)
                    {
                        if (!accessor[element.index])
                        {
                            continue;
                        }
                        
                        auto index = (vi * layout.stride + element.offset) / sizeof(float);
                        float* outData = &vertices[index];
                        readFloats(accessor[element.index], vi, outData, element.size);
                    }
                }
                
                std::shared_ptr<Mesh> mesh;
                if (primitive.indices)
                {
                    auto indexCount = primitive.indices->count;
                    std::vector<uint32_t> indices;
                    indices.resize(indexCount);
                    
                    for (cgltf_size i = 0; i < indexCount; ++i)
                    {
                        indices[i] = readIndices(primitive.indices, i);
                    }
                    
                    mesh = std::make_shared<Mesh>(layout, vertices, indices);
                }
                else
                {
                    mesh = std::make_shared<Mesh>(layout, vertices);
                }
                
                // Parsing materials
                auto material = std::make_shared<Material>();
                material->SetShaderProgram(Engine::GetInstance().GetGraphicsAPI().GetDefaultShaderProgram());
                
                if (primitive.material)
                {
                    auto gltfMaterial = primitive.material;
                    if (gltfMaterial->has_pbr_metallic_roughness)
                    {
                        auto pbr = gltfMaterial->pbr_metallic_roughness;
                        auto texture = pbr.base_color_texture.texture;
                        if (texture && texture->image)
                        {
                            if (texture->image->uri)
                            {
                                auto path = relativeFolderPath / texture->image->uri;
                                auto tex = Engine::GetInstance().GetTextureManager().GetOrLoadTexture(path.string());
                                material->SetParam("baseColorTexture", tex);
                            }
                        }
                    }
                    else if (gltfMaterial->has_pbr_specular_glossiness)
                    {
                        auto pbr = gltfMaterial->pbr_specular_glossiness;
                        auto texture = pbr.diffuse_texture.texture;
                        if (texture && texture->image)
                        {
                            if (texture->image->uri)
                            {
                                auto path = relativeFolderPath / texture->image->uri;
                                auto tex = Engine::GetInstance().GetTextureManager().GetOrLoadTexture(path.string());
                                material->SetParam("baseColorTexture", tex);
                            }
                        }
                    }
                    
                    object->AddComponent(new MeshComponent(mesh, material));
                }
            }
        }
        
        for (cgltf_size ci = 0; ci < node->children_count; ++ci)
        {
            ParseGLTFNode(node->children[ci], object, relativeFolderPath);
        }
    }
    
    auto ReadScalar = [](const cgltf_accessor* acc, cgltf_size index)
    {
        float res = 0.0f;
        cgltf_accessor_read_float(acc, index, &res, 1);
        return res;
    };
    
    auto ReadVec3 = [](const cgltf_accessor* acc, cgltf_size index)
    {
        glm::vec3 res = glm::vec3(0.0f);
        cgltf_accessor_read_float(acc, index, glm::value_ptr(res), 3);
        return res;
    };
    
    auto ReadQuat = [](const cgltf_accessor* acc, cgltf_size index)
    {
        float res[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        cgltf_accessor_read_float(acc, index, res, 4);
        return glm::quat(res[3], res[0], res[1], res[2]);
    };
    
    auto ReadTimes = [](const cgltf_accessor* acc, std::vector<float>& outTimes)
    {
        outTimes.resize(acc->count);
        for (cgltf_size i = 0; i < acc->count; ++i)
        {
            outTimes[i] = ReadScalar(acc, i);
        }
    };
    
    auto ReadOutputVec3 = [](const cgltf_accessor* acc, std::vector<glm::vec3>& outVec3)
    {
        outVec3.resize(acc->count);
        for (cgltf_size i = 0; i < acc->count; ++i)
        {
            outVec3[i] = ReadVec3(acc, i);
        }
    };
    
    auto ReadOutputQuat = [](const cgltf_accessor* acc, std::vector<glm::quat>& outQuats)
    {
        outQuats.resize(acc->count);
        for (cgltf_size i = 0; i < acc->count; ++i)
        {
            outQuats[i] = ReadQuat(acc, i);
        }
    };
    
    GameObject* GameObject::LoadGLTF(const std::string& path)
    {
        auto content = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
        if (content.empty())
        {
            return nullptr;
        }
        
        cgltf_options options = {};
        cgltf_data* data = nullptr;
        cgltf_result result = cgltf_parse(&options, content.data(), content.size(), &data);
        if (result != cgltf_result_success)
        {
            return nullptr;
        }
        
        auto fullPath = Engine::GetInstance().GetFileSystem().GetAssetsFolder() / path;
        auto fullFolderPath = fullPath.remove_filename();
        auto relativeFolderPath = std::filesystem::path(path).remove_filename();
        
        result = cgltf_load_buffers(&options, data, fullPath.remove_filename().string().c_str());
        if (result != cgltf_result_success)
        {
            cgltf_free(data);
            return nullptr;
        }
        
        auto resultObject = Engine::GetInstance().GetScene()->CreateGameObject("Result");
        auto scene = &data->scenes[0];
        
        for (cgltf_size i = 0; i < scene->nodes_count; ++i)
        {
            auto node = scene->nodes[i];
            ParseGLTFNode(node, resultObject, relativeFolderPath);
        }
        
        //Parse Animations
        std::vector<std::shared_ptr<AnimationClip>> clips;
        for (cgltf_size ai = 0; ai < data->animations_count; ++ai)
        {
            auto& animation = data->animations[ai];
            
            auto clip = std::make_shared<AnimationClip>();
            clip->name = animation.name ? animation.name : "None";
            clip->duration = 0.0f;
            
            std::unordered_map<cgltf_node*, size_t> trackIndexOf;
            
            auto GetOrCreateTrack = [&](cgltf_node* node) -> TransformTrack&
            {
                auto it = trackIndexOf.find(node);
                if (it != trackIndexOf.end())
                {
                    return clip->tracks[it->second];
                }
                
                TransformTrack track;
                track.targetName = node->name;
                clip->tracks.push_back(track);
                size_t index = clip->tracks.size() - 1;
                trackIndexOf[node] = index;
                return clip->tracks[index];
            };
            
            for (cgltf_size ci = 0; ci < animation.channels_count; ++ci)
            {
                auto& channel = animation.channels[ci];
                auto sampler = channel.sampler;
                
                if (!channel.target_node || !sampler || !sampler->input || !sampler->output)
                {
                    continue;
                }
                
                std::vector<float> times;
                ReadTimes(sampler->input, times);
                
                auto& track = GetOrCreateTrack(channel.target_node);

                switch (channel.target_path)
                {
                    case cgltf_animation_path_type_translation:
                        {
                            std::vector<glm::vec3> positions;
                            ReadOutputVec3(sampler->output, positions);
                            track.positions.resize(times.size());
                            for (size_t i = 0; i < times.size(); ++i)
                            {
                                track.positions[i].time = times[i];
                                track.positions[i].value = positions[i];
                            }
                            break;
                        }
                    case cgltf_animation_path_type_rotation:
                        {
                            std::vector<glm::quat> rotations;
                            ReadOutputQuat(sampler->output, rotations);
                            track.rotations.resize(times.size());
                            for (size_t i = 0; i < times.size(); ++i)
                            {
                                track.rotations[i].time = times[i];
                                track.rotations[i].value = rotations[i];
                            }
                            break;
                        }
                    case cgltf_animation_path_type_scale:
                        {
                            std::vector<glm::vec3> scales;
                            ReadOutputVec3(sampler->output, scales);
                            track.scales.resize(times.size());
                            for (size_t i = 0; i < times.size(); ++i)
                            {
                                track.scales[i].time = times[i];
                                track.scales[i].value = scales[i];
                            }
                            break;
                        }
                    default:
                        break;
                    
                }
                
                clip->duration = std::max(clip->duration, times.back());
            }
            
            clips.push_back(std::move(clip));
        }
        
        if (!clips.empty())
        {
            auto animComponent = new AnimationComponent();
            resultObject->AddComponent(animComponent);
            for (auto& clip : clips)
            {
                animComponent->RegisterAnimationClip(clip->name, clip);
            }
        }
        
        cgltf_free(data);
        
        return resultObject;
    }

    glm::mat4 GameObject::GetLocalTransform() const
    {
        glm::mat4 mat = glm::mat4(1.0f);
        
        //Translation
        mat = glm::translate(mat, m_position);
        
        //Rotation
        mat = mat * glm::mat4_cast(m_rotation);
        
        //Scale
        mat = glm::scale(mat, m_scale);
        
        return mat;
    }

    glm::mat4 GameObject::GetWorldTransform() const
    {
        if (m_parent)
        {
            return m_parent->GetWorldTransform() * GetLocalTransform();
        }
        else
        {
            return GetLocalTransform();
        }       
    }
}
