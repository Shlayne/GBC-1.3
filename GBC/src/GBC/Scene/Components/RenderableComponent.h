#pragma once

#include "glm/glm.hpp"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct RenderableComponent
	{
		RenderableComponent() = default;
		RenderableComponent(const Ref<Texture>& texture);
		RenderableComponent(Ref<Texture>&& texture) noexcept;
		RenderableComponent& operator=(const Ref<Texture>& texture);
		RenderableComponent& operator=(Ref<Texture>&& texture) noexcept;

		inline operator Ref<Texture>&() { return texture; }
		inline operator const Ref<Texture>&() const { return texture; }

		std::string filepath;
		Ref<Texture> texture;
	};
}
