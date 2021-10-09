#pragma once

#include "GBC/Model/Mesh3D.h"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	struct Mesh3DComponent
	{
		Ref<Mesh3D> mesh;

		// TODO: move this to MaterialComponent
		Ref<Texture2D> texture;
	};
}
