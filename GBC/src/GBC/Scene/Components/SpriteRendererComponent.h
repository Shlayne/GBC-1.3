#pragma once

#include "GBC/Rendering/Texture.h"
#include <glm/glm.hpp>

namespace gbc
{
	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f };
		Ref<Texture2D> texture;
		glm::vec2 tilingFactor{ 1.0f };
	};
}
