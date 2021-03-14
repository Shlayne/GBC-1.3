#pragma once

#include "glm/glm.hpp"
#include "GBC/Rendering/Basic/BasicRenderable.h"

namespace gbc
{
	struct RenderableComponent
	{
		RenderableComponent() = default;
		RenderableComponent(const BasicRenderable& renderable);
		RenderableComponent(BasicRenderable&& renderable) noexcept;
		RenderableComponent& operator=(const BasicRenderable& renderable);
		RenderableComponent& operator=(BasicRenderable&& renderable) noexcept;

		inline operator BasicRenderable&() { return renderable; }
		inline operator const BasicRenderable&() const { return renderable; }

		BasicRenderable renderable;
	};
}
