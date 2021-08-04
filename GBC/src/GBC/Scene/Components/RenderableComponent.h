#pragma once

#include "glm/glm.hpp"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct RenderableComponent
	{
		RenderableComponent() noexcept = default;
		RenderableComponent(const RenderableComponent&) noexcept = default;

		glm::vec4 color{1.0f};
		Ref<Texture> texture;
		float tilingFactor = 1.0f;
	};
}
