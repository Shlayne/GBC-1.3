#include "gbcpch.h"
#include "MeshComponent.h"

namespace gbc
{
	MeshComponent::MeshComponent(MeshComponent&& mesh) noexcept
		: mesh(std::move(mesh.mesh)) {}

	MeshComponent& MeshComponent::operator=(MeshComponent&& mesh) noexcept
	{
		if (this != &mesh)
			this->mesh = std::move(mesh.mesh);
		return *this;
	}

	MeshComponent::MeshComponent(const Ref<BasicMesh>& mesh)
		: mesh(mesh) {}

	MeshComponent::MeshComponent(Ref<BasicMesh>&& mesh) noexcept
		: mesh(std::move(mesh)) {}

	MeshComponent& MeshComponent::operator=(Ref<BasicMesh>&& mesh) noexcept
	{
		this->mesh = std::move(mesh);
		return *this;
	}

	MeshComponent::MeshComponent(const OBJModel& model)
		: filepath(model.filepath), mesh(CreateRef<BasicMesh>(model)) {}

	MeshComponent::MeshComponent(OBJModel&& model) noexcept
		: filepath(std::move(model.filepath)), mesh(CreateRef<BasicMesh>(std::move(model))) {}

	MeshComponent& MeshComponent::operator=(OBJModel&& model) noexcept
	{
		filepath = std::move(model.filepath);
		mesh = CreateRef<BasicMesh>(std::move(model));
		return *this;
	}
}
