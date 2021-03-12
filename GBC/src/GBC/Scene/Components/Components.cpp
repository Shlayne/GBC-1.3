#include "gbcpch.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "MeshComponent.h"

namespace gbc
{
	TransformComponent::TransformComponent(const glm::mat4& transform)
		: transform(transform) {}

	RenderableComponent::RenderableComponent(const BasicRenderable* renderable)
		: renderable(renderable) {}

	MeshComponent::MeshComponent(const BasicModel* model)
		: model(model) {}
}
