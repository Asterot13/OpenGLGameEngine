#include "Audio.h"

#include "Engine.h"
#include "miniaudio.h"

namespace eng
{
    Audio::~Audio()
    {
        if (m_sound)
        {
            ma_sound_uninit(m_sound.get());
        }
        
        if (m_decoder)
        {
            ma_decoder_uninit(m_decoder.get());
        }
    }

    void Audio::SetPosition(const glm::vec3& position)
    {
        if (m_sound)
        {
            ma_sound_set_position(m_sound.get(), position.x, position.y, position.z);
        }
    }

    void Audio::Play(bool loop)
    {
        if (m_sound)
        {
            ma_sound_start(m_sound.get());
            auto looping = loop ? MA_TRUE : MA_FALSE;
            ma_sound_set_looping(m_sound.get(), looping);
        }
    }

    void Audio::Stop()
    {
        if (m_sound)
        {
            ma_sound_stop(m_sound.get());
            ma_sound_seek_to_pcm_frame(m_sound.get(), 0);
        }
    }

    bool Audio::IsPlaying()
    {
        if (m_sound)
        {
            return ma_sound_is_playing(m_sound.get());
        }
        return false;
    }

    void Audio::SetVolume(float volume)
    {
        if (m_sound)
        {
            ma_sound_set_volume(m_sound.get(), std::clamp(volume, 0.0f, 1.0f));
        }
    }

    float Audio::GetVolume()
    {
        if (m_sound)
        {
            return ma_sound_get_volume(m_sound.get());
        }
        return 0.0f;
    }

    std::shared_ptr<Audio> Audio::Load(const std::string& filePath)
    {
        auto engine = Engine::GetInstance().GetAudioManager().GetEngine();
        if (!engine)
        {
            return nullptr;
        }

        auto buffer = Engine::GetInstance().GetFileSystem().LoadAssetFile(filePath);
        if (buffer.empty())
        {
            return nullptr;
        }
        
        auto audio = std::make_shared<Audio>();
        audio->m_buffer = buffer;
        auto decoder = std::make_unique<ma_decoder>();
        auto result = ma_decoder_init_memory(audio->m_buffer.data(), audio->m_buffer.size(), nullptr, decoder.get());
        if (result != MA_SUCCESS)
        {
            return nullptr;
        }
        audio->m_decoder = std::move(decoder);
        
        auto sound = std::make_unique<ma_sound>();
        result = ma_sound_init_from_data_source(engine, audio->m_decoder.get(), 0, nullptr, sound.get());
        if (result != MA_SUCCESS)
        {
            return nullptr;
        }
        audio->m_sound = std::move(sound);
        
        ma_sound_set_spatialization_enabled(audio->m_sound.get(), MA_TRUE);
        return audio;
    }
}
