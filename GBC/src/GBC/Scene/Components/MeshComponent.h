#pragma once

#include "GBC/Core/Core.h"
#include "GBC/IO/OBJLoader.h"
#include "GBC/Rendering/Basic/BasicMesh.h"

namespace gbc
{
	struct MeshComponent
	{
		MeshComponent() noexcept = default;
		MeshComponent(MeshComponent&& mesh) noexcept;
		MeshComponent& operator=(MeshComponent&& mesh) noexcept;

		MeshComponent(const Ref<BasicMesh>& mesh);
		MeshComponent(Ref<BasicMesh>&& mesh) noexcept;
		MeshComponent& operator=(Ref<BasicMesh>&& mesh) noexcept;

		MeshComponent(const OBJModel& model);
		MeshComponent(OBJModel&& model) noexcept;
		MeshComponent& operator=(OBJModel&& model) noexcept;

		inline operator Ref<BasicMesh>&() noexcept { return mesh; }
		inline operator const Ref<BasicMesh>&() const noexcept { return mesh; }

		std::string filepath;
		Ref<BasicMesh> mesh;
	};
}
