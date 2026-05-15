#pragma once
#include <glm/vec3.hpp>
#include "scene/Component.h"

namespace eng
{
	class LightComponent : public Component
	{
		COMPONENT(LightComponent)
	public:
		void Update(float DeltaTime) override;
		
		const glm::vec3& GetColor() const;
		void SetColor(const glm::vec3& color);
		
	private:
		glm::vec3 m_color = glm::vec3(1.0f);
		
	};
}
