#pragma once

#include "glm/glm.hpp"
#include "GBC/Rendering/Basic/BasicRenderable.h"

namespace gbc
{
	struct RenderableComponent
	{
		RenderableComponent() = default;
		RenderableComponent(const RenderableComponent&) = default;
		RenderableComponent(const BasicRenderable* renderable);

		inline operator const BasicRenderable&() const { return *renderable; }

		const BasicRenderable* renderable;
	};
}
