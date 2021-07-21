#pragma once

#include "glm/glm.hpp"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct RenderableComponent
	{
		RenderableComponent() noexcept = default;
		RenderableComponent(const Ref<Texture>& texture) noexcept;
		RenderableComponent(Ref<Texture>&& texture) noexcept;
		RenderableComponent& operator=(const Ref<Texture>& texture) noexcept;
		RenderableComponent& operator=(Ref<Texture>&& texture) noexcept;

		inline operator Ref<Texture>&() noexcept { return texture; }
		inline operator const Ref<Texture>&() const noexcept { return texture; }

		std::string filepath;
		Ref<Texture> texture;
	};
}
