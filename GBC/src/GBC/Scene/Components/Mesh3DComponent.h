#pragma once

#include "GBC/Model/Mesh3D.h"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct Mesh3DComponent
	{
		Ref<Mesh3D> mesh;

		// TODO: move these to MaterialComponent
		glm::vec4 tintColor{ 1.0f };
		Ref<Texture2D> texture;
	};
}
