#include "AudioManager.h"

#include "miniaudio.h"

namespace eng
{
    AudioManager::AudioManager() = default;

    AudioManager::~AudioManager()
    {
        if (m_engine)
        {
            ma_engine_uninit(m_engine.get());
        }
    }

    bool AudioManager::Init()
    {
        if (m_engine)
        {
            return true;
        }

        auto engine = std::make_unique<ma_engine>();
        if (ma_engine_init(nullptr, engine.get()) != MA_SUCCESS)
        {
            return false;
        }

        m_engine = std::move(engine);
        return true;
    }

    ma_engine* AudioManager::GetEngine()
    {
        return m_engine.get();
    }

    void AudioManager::SetListenerPosition(const glm::vec3& position)
    {
        if (m_engine)
        {
            ma_engine_listener_set_position(m_engine.get(), 0, position.x, position.y, position.z);
        }
    }
}
