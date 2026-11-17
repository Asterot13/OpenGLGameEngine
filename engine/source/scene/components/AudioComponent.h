#pragma once
#include "audio/Audio.h"
#include "scene/Component.h"

namespace eng
{
    class AudioComponent : public Component
    {
        COMPONENT(AudioComponent)
        
    public:
        void Update(float DeltaTime) override;
        void LoadProperties(const nlohmann::json& properties) override;
        
        void RegisterAudio(const std::string& name, std::shared_ptr<Audio>& audioClip);
        void Play(const std::string& name, bool loop = false);
        void Stop(const std::string& name);
        bool IsPlaying(const std::string& name);
        
    private:
        std::unordered_map<std::string, std::shared_ptr<Audio>> m_audioClips;
    };
}
