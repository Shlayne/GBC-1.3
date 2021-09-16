#include "gbcpch.h"
#include "SpriteRendererComponent.h"

namespace gbc
{
	SpriteRendererComponent::SpriteRendererComponent(const SpriteRendererComponent& component)
		: color(component.color), texture(component.texture), tilingFactor(component.tilingFactor) {}
}
