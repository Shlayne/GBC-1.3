#pragma once

#include <glm/glm.hpp>
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& component);

		glm::vec4 color{ 1.0f };
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;
	};
}
