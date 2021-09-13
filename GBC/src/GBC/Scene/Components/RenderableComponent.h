#pragma once

#include "glm/glm.hpp"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct RenderableComponent
	{
		RenderableComponent() = default;
		RenderableComponent(const RenderableComponent& component);

		glm::vec4 color{ 1.0f };
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;
	};
}
