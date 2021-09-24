#pragma once

#include <glm/glm.hpp>
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		glm::vec4 color{ 1.0f };
		Ref<Texture2D> texture;
		glm::vec2 tilingFactor{ 1.0f };
	};
}
