#pragma once

#include "GBC/Rendering/Basic/BasicModel.h"

namespace gbc
{
	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const BasicModel* model);

		inline operator const BasicModel&() const { return *model; }

		const BasicModel* model;
	};

}
