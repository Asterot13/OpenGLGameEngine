#include "scene/components/AudioComponent.h"

#include <scene/GameObject.h>

namespace eng
{
    void AudioComponent::Update(float DeltaTime)
    {
        auto pos = m_owner->GetWorldPosition();
        for (auto& audioClip : m_audioClips)
        {
            if (audioClip.second && audioClip.second->IsPlaying())
            {
                audioClip.second->SetPosition(pos);
            }
        }
    }

    void AudioComponent::LoadProperties(const nlohmann::json& properties)
    {
        if (properties.contains("audio"))
        {
            auto& audioClipsObject = properties["audio"];
            for (auto& audioClip : audioClipsObject)
            {
                std::string name = audioClip.value("name", "noname");
                std::string filepath = audioClip.value("filepath", "");
                auto audio = Audio::Load(filepath);
                if (audio)
                {
                    float volume = audioClip.value("volume", 1.0f);
                    audio->SetVolume(volume);
                    RegisterAudio(name, audio);
                }
            }
        }
    }

    void AudioComponent::RegisterAudio(const std::string& name, std::shared_ptr<Audio>& audioClip)
    {
        if (!audioClip)
        {
            return;
        }

        m_audioClips[name] = audioClip;
    }

    void AudioComponent::Play(const std::string& name, bool loop)
    {
        auto it = m_audioClips.find(name);
        if (it != m_audioClips.end())
        {
            if (it->second)
            {
                it->second->SetPosition(m_owner->GetWorldPosition());
                it->second->Play(loop);
            }
        }
    }

    void AudioComponent::Stop(const std::string& name)
    {
        auto it = m_audioClips.find(name);
        if (it != m_audioClips.end())
        {
            if (it->second)
            {
                it->second->Stop();
            }
        }
    }

    bool AudioComponent::IsPlaying(const std::string& name)
    {
        auto it = m_audioClips.find(name);
        if (it != m_audioClips.end())
        {
            if (it->second)
            {
                return it->second->IsPlaying();
            }
        }
        
        return false;
    }
}
