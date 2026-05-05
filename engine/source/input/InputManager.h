#pragma once
#include <array>
#include <glm/vec2.hpp>

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
      bool IsKeyPressed(int key) const;
      
      void SetMouseButtonPressed(int button, bool pressed);
      bool IsMouseButtonPressed(int button) const;
      
      void SetMousePositionPrev(const glm::vec2& position);
      const glm::vec2 GetMousePositionPrev() const;
      
      void SetMousePositionCurrent(const glm::vec2& position);
      const glm::vec2 GetMousePositionCurrent() const;
      
    private:
      friend class Engine;
      
      std::array<bool, MaxKeys> m_keys = { false };
      std::array<bool, 16> m_mouseKeys = { false };
      
      glm::vec2 m_mousePositionPrev = glm::vec2(0.0f);
      glm::vec2 m_mousePositionCurrent = glm::vec2(0.0f);
    };
}
