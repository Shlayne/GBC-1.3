#include "gbcpch.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "TagComponent.h"

namespace gbc
{
	TransformComponent::TransformComponent(const glm::mat4& transform)
		: transform(transform) {}
	TransformComponent& TransformComponent::operator=(const glm::mat4& transform)
	{
		this->transform = transform;
		return *this;
	}

	TagComponent::TagComponent(const std::string& tag)
		: tag(tag) {}
	TagComponent::TagComponent(std::string&& tag) noexcept
		: tag(std::move(tag)) {}
	TagComponent& TagComponent::operator=(const std::string& tag)
	{
		this->tag = tag;
		return *this;
	}
	TagComponent& TagComponent::operator=(std::string&& tag) noexcept
	{
		this->tag = std::move(tag);
		return *this;
	}

	RenderableComponent::RenderableComponent(const BasicRenderable& renderable)
		: renderable(renderable) {}
	RenderableComponent::RenderableComponent(BasicRenderable&& renderable) noexcept
		: renderable(std::move(renderable)) {}
	RenderableComponent& RenderableComponent::operator=(const BasicRenderable& renderable)
	{
		this->renderable = renderable;
		return *this;
	}
	RenderableComponent& RenderableComponent::operator=(BasicRenderable&& renderable) noexcept
	{
		this->renderable = std::move(renderable);
		return *this;
	}

	MeshComponent::MeshComponent(MeshComponent&& mesh) noexcept
		: model(std::move(mesh.model)) {}
	MeshComponent& MeshComponent::operator=(MeshComponent&& mesh) noexcept
	{
		model = std::move(mesh.model);
		return *this;
	}
	MeshComponent::MeshComponent(const BasicModel& model)
		: model(model) {}
	MeshComponent::MeshComponent(BasicModel&& model) noexcept
		: model(std::move(model)) {}

	CameraComponent::CameraComponent(const SceneCamera& camera)
		: camera(camera) {}
	CameraComponent& CameraComponent::operator=(const SceneCamera& camera)
	{
		this->camera = camera;
		return *this;
	}
}
