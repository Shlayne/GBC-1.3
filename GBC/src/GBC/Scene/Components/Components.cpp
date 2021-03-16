#include "gbcpch.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "TagComponent.h"

namespace gbc
{
	TransformComponent::TransformComponent(const glm::vec3& translation)
		: translation(translation) {}
	
	TransformComponent::operator glm::mat4() const
	{
		return glm::translate(glm::mat4(1.0f), translation) *
			   glm::toMat4(glm::quat(rotation)) *
			   glm::scale(glm::mat4(1.0f), scale);
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

	RenderableComponent::RenderableComponent(const Ref<Texture>& texture)
		: texture(texture) {}
	RenderableComponent::RenderableComponent(Ref<Texture>&& texture) noexcept
		: texture(std::move(texture)) {}
	RenderableComponent& RenderableComponent::operator=(const Ref<Texture>& texture)
	{
		this->texture = texture;
		return *this;
	}
	RenderableComponent& RenderableComponent::operator=(Ref<Texture>&& texture) noexcept
	{
		this->texture = std::move(texture);
		return *this;
	}

	MeshComponent::MeshComponent(MeshComponent&& mesh) noexcept
		: mesh(std::move(mesh.mesh)) {}
	MeshComponent& MeshComponent::operator=(MeshComponent&& mesh) noexcept
	{
		mesh = std::move(mesh.mesh);
		return *this;
	}
	MeshComponent::MeshComponent(const Ref<BasicMesh>& mesh)
		: mesh(mesh) {}
	MeshComponent::MeshComponent(Ref<BasicMesh>&& mesh) noexcept
		: mesh(std::move(mesh)) {}
	MeshComponent& MeshComponent::operator=(Ref<BasicMesh>&& mesh) noexcept
	{
		mesh = std::move(mesh);
		return *this;
	}

	CameraComponent::CameraComponent(const SceneCamera& camera)
		: camera(camera) {}
	CameraComponent& CameraComponent::operator=(const SceneCamera& camera)
	{
		this->camera = camera;
		return *this;
	}
}
