#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Model/Model3D.h"

namespace gbc
{
	class ModelLoader3D
	{
	public:
		virtual ~ModelLoader3D() noexcept = default;

		static Ref<Model3D> Load(const std::filesystem::path& filepath);
	private:
		static Scope<ModelLoader3D> loader;

		virtual Ref<Model3D> LoadModel(const std::filesystem::path& filepath) = 0;
	};
}
