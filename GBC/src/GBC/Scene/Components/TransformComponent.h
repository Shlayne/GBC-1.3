#pragma once

#include <glm/glm.hpp>

namespace gbc
{
	struct TransformComponent
	{
		constexpr TransformComponent() noexcept = default;
		constexpr TransformComponent(const TransformComponent&) noexcept = default;
		constexpr TransformComponent(const glm::vec3& translation) noexcept : translation(translation) {}

		operator glm::mat4() const noexcept;

		glm::vec3 translation{ 0.0f };
		glm::vec3 rotation{ 0.0f };
		glm::vec3 scale{ 1.0f };
	};
}
