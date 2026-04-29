#pragma once
#include <array>

namespace eng
{
    constexpr int MaxKeys = 512;

    class InputManager {
      InputManager() = default;
      InputManager(const InputManager&) = delete;
      InputManager(InputManager&&) = delete;
      InputManager& operator=(const InputManager&) = delete;
      InputManager& operator=(InputManager&&) = delete;
      
    public:
      void SetKeyPressed(int key, bool pressed);
      bool IsKeyPressed(int key);
      
    private:
      friend class Engine;
      std::array<bool, MaxKeys> m_keys = { false };
    };
}
