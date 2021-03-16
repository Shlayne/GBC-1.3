#pragma once

#include "GBC/Rendering/Basic/BasicMesh.h"

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

		inline operator Ref<BasicMesh>&() { return mesh; }
		inline operator const Ref<BasicMesh>&() const { return mesh; }

		Ref<BasicMesh> mesh;
	};
}
