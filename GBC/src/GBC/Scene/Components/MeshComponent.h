#pragma once

#include "GBC/Rendering/Basic/BasicMesh.h"
#include "GBC/IO/OBJLoader.h"

namespace gbc
{
	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(MeshComponent&& mesh) noexcept;
		MeshComponent& operator=(MeshComponent&& mesh) noexcept;

		MeshComponent(const Ref<BasicMesh>& mesh);
		MeshComponent(Ref<BasicMesh>&& mesh) noexcept;
		MeshComponent& operator=(Ref<BasicMesh>&& mesh) noexcept;

		MeshComponent(const OBJModel& mesh);
		MeshComponent(OBJModel&& mesh) noexcept;
		MeshComponent& operator=(OBJModel&& mesh) noexcept;

		inline operator Ref<BasicMesh>&() { return mesh; }
		inline operator const Ref<BasicMesh>&() const { return mesh; }

		std::string filepath;
		Ref<BasicMesh> mesh;
	};
}
