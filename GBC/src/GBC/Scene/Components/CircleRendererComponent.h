#pragma once

#include <glm/glm.hpp>

namespace gbc
{
	struct CircleRendererComponent
	{
		constexpr CircleRendererComponent() noexcept = default;
		constexpr CircleRendererComponent(const CircleRendererComponent&) noexcept = default;
		constexpr CircleRendererComponent(const glm::vec4& color) noexcept : color(color) {}

		float thickness = 1.0f;
		glm::vec4 color{ 1.0f };
	};
}
