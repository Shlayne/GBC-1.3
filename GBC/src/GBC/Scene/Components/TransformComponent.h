#pragma once

#include "glm/glm.hpp"

namespace gbc
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform);
		TransformComponent& operator=(const glm::mat4& transform);

		inline operator glm::mat4&() { return transform; }
		inline operator const glm::mat4&() const { return transform; }

		glm::mat4 transform{1.0f};
	};
}
