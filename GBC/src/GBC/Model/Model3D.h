#pragma once

#include "GBC/Core/Asset.h"
#include "GBC/Core/Core.h"
#include "Mesh3D.h"

namespace gbc
{
	class Model3D : public Asset
	{
	public:
		Model3D(UUID id, const std::filesystem::path& filepath) : id(id), filepath(filepath) {}
	public:
		virtual UUID GetUUID() const override { return id; }
		virtual const std::filesystem::path& GetFilepath() const override { return filepath; }
	public:
		inline std::vector<Mesh3D>& GetMeshes() noexcept { return meshes; }
		inline const std::vector<Mesh3D>& GetMeshes() const noexcept { return meshes; }
	private:
		UUID id;
		std::filesystem::path filepath; // TODO: this should most likely be removed
		std::vector<Mesh3D> meshes;
	};
}
