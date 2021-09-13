#include "gbcpch.h"
#include "RenderableComponent.h"

namespace gbc
{
	RenderableComponent::RenderableComponent(const RenderableComponent& component)
		: color(component.color), texture(component.texture), tilingFactor(component.tilingFactor) {}
}
