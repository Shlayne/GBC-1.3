#include "gbcpch.h"
#include "ModelLoader3D.h"
#include "Loaders/AssimpModelLoader3D.h"

namespace gbc
{
	Scope<ModelLoader3D> ModelLoader3D::loader = CreateScope<AssimpModelLoader3D>();

	Ref<Model3D> ModelLoader3D::Load(const std::filesystem::path& filepath)
	{
		GBC_CORE_ASSERT(loader, "Model Loader 3D is nullptr!");
		return loader->LoadModel(filepath);
	}
}
