#include "gbcpch.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "TagComponent.h"

namespace gbc
{
	CameraComponent::CameraComponent(const SceneCamera& camera) noexcept
		: camera(camera) {}
	CameraComponent& CameraComponent::operator=(const SceneCamera& camera) noexcept
	{
		this->camera = camera;
		return *this;
	}

	TagComponent::TagComponent(const std::string& tag) noexcept
		: tag(tag) {}
	TagComponent::TagComponent(std::string&& tag) noexcept
		: tag(std::move(tag)) {}
	TagComponent& TagComponent::operator=(const std::string& tag) noexcept
	{
		this->tag = tag;
		return *this;
	}
	TagComponent& TagComponent::operator=(std::string&& tag) noexcept
	{
		this->tag = std::move(tag);
		return *this;
	}

	TransformComponent::TransformComponent(const glm::vec3& translation) noexcept
		: translation(translation) {}
	
	TransformComponent::operator glm::mat4() const noexcept
	{
		return glm::translate(glm::mat4(1.0f), translation) *
			   glm::toMat4(glm::quat(rotation)) *
			   glm::scale(glm::mat4(1.0f), scale);
	}

	MeshComponent::MeshComponent(MeshComponent&& mesh) noexcept
		: mesh(std::move(mesh.mesh)) {}
	MeshComponent& MeshComponent::operator=(MeshComponent&& mesh) noexcept
	{
		mesh = std::move(mesh.mesh);
		return *this;
	}
	MeshComponent::MeshComponent(const Ref<BasicMesh>& mesh) noexcept
		: mesh(mesh) {}
	MeshComponent::MeshComponent(Ref<BasicMesh>&& mesh) noexcept
		: mesh(std::move(mesh)) {}
	MeshComponent& MeshComponent::operator=(Ref<BasicMesh>&& mesh) noexcept
	{
		mesh = std::move(mesh);
		return *this;
	}
	MeshComponent::MeshComponent(const OBJModel& model) noexcept
		: filepath(model.filepath), mesh(CreateRef<BasicMesh>(model)) {}
	MeshComponent::MeshComponent(OBJModel&& model) noexcept
		: filepath(std::move(model.filepath)), mesh(std::move(CreateRef<BasicMesh>(std::move(model)))) {}
	MeshComponent& MeshComponent::operator=(OBJModel&& model) noexcept
	{
		filepath = std::move(model.filepath);
		mesh = CreateRef<BasicMesh>(std::move(model));
		return *this;
	}

	RenderableComponent::RenderableComponent(const Ref<Texture>& texture) noexcept
		: texture(texture)
	{
		if (this->texture && this->texture->GetTexture())
			filepath = this->texture->GetTexture()->GetFilePath();
	}
	RenderableComponent::RenderableComponent(Ref<Texture>&& texture) noexcept
		: texture(std::move(texture))
	{
		if (this->texture && this->texture->GetTexture())
			filepath = this->texture->GetTexture()->GetFilePath();
	}
	RenderableComponent& RenderableComponent::operator=(const Ref<Texture>& texture) noexcept
	{
		this->texture = texture;
		if (this->texture && this->texture->GetTexture())
			filepath = this->texture->GetTexture()->GetFilePath();
		else
			filepath.clear();
		return *this;
	}
	RenderableComponent& RenderableComponent::operator=(Ref<Texture>&& texture) noexcept
	{
		this->texture = std::move(texture);
		if (this->texture && this->texture->GetTexture())
			filepath = this->texture->GetTexture()->GetFilePath();
		else
			filepath.clear();
		return *this;
	}
}
