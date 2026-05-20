#include "AnimationComponent.h"

#include <cmath>

#include "Engine.h"
#include "scene/GameObject.h"

namespace eng
{
    void AnimationComponent::Update(float DeltaTime)
    {
        if (!m_clip)
        {
            return;
        }

        if (!m_isPlaying)
        {
            return;
        }

        if (m_clip->duration <= 0.0f)
        {
            return;
        }

        m_time += DeltaTime;

        if (m_time >= m_clip->duration)
        {
            if (m_loop)
            {
                m_time = std::fmod(m_time, m_clip->duration);
            }
            else
            {
                m_isPlaying = false;
                m_time = 0.0f;
                return;
            }
        }

        for (auto& binding : m_objectBindings)
        {
            auto& obj = binding.first;
            auto& trackIndicies = binding.second->trackIndices;
            for (auto& trackIndex : trackIndicies)
            {
                auto& track = m_clip->tracks[trackIndex];
                //Positions
                if (!track.positions.empty())
                {
                    auto pos = Interpolate(track.positions, m_time);
                    obj->SetPosition(pos);
                }
                //Rotations
                if (!track.rotations.empty())
                {
                    auto rot = Interpolate(track.rotations, m_time);
                    obj->SetRotation(rot);
                }
                //Scales
                if (!track.scales.empty())
                {
                    auto scale = Interpolate(track.scales, m_time);
                    obj->SetScale(scale);
                }
            }
        }
    }

    void AnimationComponent::SetClip(AnimationClip* clip)
    {
        m_clip = clip;
        BuildBindings();
    }

    void AnimationComponent::RegisterAnimationClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip)
    {
        m_animationClips[name] = clip;
    }

    void AnimationComponent::Play(const std::string& name, bool loop)
    {
        if (m_clip && m_clip->name == name)
        {
            m_time = 0.0f;
            m_isPlaying = true;
            m_loop = loop;
        }
        else
        {
            auto it = m_animationClips.find(name);
            if (it != m_animationClips.end())
            {
                SetClip(it->second.get());
                m_time = 0.0f;
                m_isPlaying = true;
                m_loop = loop;
            }
        }
    }

    void AnimationComponent::BuildBindings()
    {
        m_objectBindings.clear();
        if (!m_clip)
        {
            return;
        }
        
        for (size_t i = 0; i < m_clip->tracks.size(); ++i)
        {
            auto& track = m_clip->tracks[i];
            auto targetObject = m_owner->FindChildByName(track.targetName);
            if (targetObject)
            {
                auto it = m_objectBindings.find(targetObject);
                if (it != m_objectBindings.end())
                {
                    it->second->trackIndices.push_back(i);
                }
                else
                {
                    auto binding = std::make_unique<ObjectBinding>();
                    binding->gameObject = targetObject;
                    binding->trackIndices.push_back(i);
                    m_objectBindings.emplace(targetObject, std::move(binding));
                }
            }           
        }
    }

    glm::vec3 AnimationComponent::Interpolate(const std::vector<KeyFrameVec3>& keyFrames, float time)
    {
        if (keyFrames.empty())
        {
            return glm::vec3(0.0f);
        }

        if (keyFrames.size() == 1)
        {
            return keyFrames[0].value;
        }

        if (time <= keyFrames.front().time)
        {
            return keyFrames.front().value;
        }

        if (time >= keyFrames.back().time)
        {
            return keyFrames.back().value;
        }

        size_t i1 = 1;
        for (size_t i = 1; i < keyFrames.size(); ++i)
        {
            if (time <= keyFrames[i].time)
            {
                i1 = i;
                break;
            }
        }

        size_t i0 = i1 - 1;

        if (time >= keyFrames[i0].time && time <= keyFrames[i1].time)
        {
            const float deltaTime = keyFrames[i1].time - keyFrames[i0].time;
            if (deltaTime <= 0.0f)
            {
                return keyFrames[i1].value;
            }
            const float koef = (time - keyFrames[i0].time) / deltaTime;
            return glm::mix(keyFrames[i0].value, keyFrames[i1].value, koef);
        }
        
        return keyFrames.back().value;
    }

    glm::quat AnimationComponent::Interpolate(const std::vector<KeyFrameQuat>& keyFrames, float time)
    {
        if (keyFrames.empty())
        {
            return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }

        if (keyFrames.size() == 1)
        {
            return keyFrames[0].value;
        }

        if (time <= keyFrames.front().time)
        {
            return keyFrames.front().value;
        }

        if (time >= keyFrames.back().time)
        {
            return keyFrames.back().value;
        }
        
        size_t i1 = 1;
        for (size_t i = 1; i < keyFrames.size(); ++i)
        {
            if (time <= keyFrames[i].time)
            {
                i1 = i;
                break;
            }
        }

        size_t i0 = i1 - 1;
        
        if (time >= keyFrames[i0].time && time <= keyFrames[i1].time)
        {
            const float deltaTime = keyFrames[i1].time - keyFrames[i0].time;
            if (deltaTime <= 0.0f)
            {
                return keyFrames[i1].value;
            }
            const float koef = (time - keyFrames[i0].time) / deltaTime;
            return glm::normalize(glm::slerp(keyFrames[i0].value, keyFrames[i1].value, koef));
        }
        
        return keyFrames.back().value;
    }
}
