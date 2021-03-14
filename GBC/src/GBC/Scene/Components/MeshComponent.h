#pragma once

#include "GBC/Rendering/Basic/BasicModel.h"

namespace gbc
{
	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(MeshComponent&& mesh) noexcept;
		MeshComponent& operator=(MeshComponent&& mesh) noexcept;

		MeshComponent(const BasicModel& model);
		MeshComponent(BasicModel&& model) noexcept;

		inline operator BasicModel&() { return model; }
		inline operator const BasicModel&() const { return model; }

		BasicModel model;
	};
}
