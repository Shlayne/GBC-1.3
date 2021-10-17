#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Core/UUID.h"
#include "Mesh3D.h"
#include <filesystem>

// TODO: make it so that these aren't required
struct aiNode;
struct aiMesh;
struct aiScene;

namespace gbc
{
	class Model3D
	{
	public:
		Model3D(const Ref<Mesh3D>& mesh, const std::filesystem::path& id);
		Model3D(const std::filesystem::path& filepath, UUID uuid);

		static Ref<Model3D> Create(const Ref<Mesh3D>& mesh, const std::filesystem::path& id);
		static Ref<Model3D> Create(const std::filesystem::path& filepath, UUID uuid);
	public:
		inline const std::filesystem::path& GetFilepath() const noexcept { return filepath; }
		inline UUID GetUUID() const noexcept { return uuid; }

		inline const std::vector<Ref<Mesh3D>>& GetMeshes() const noexcept { return meshes; }
	private:
		void Load();
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh3D ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		std::filesystem::path filepath;
		UUID uuid;

		std::vector<Ref<Mesh3D>> meshes;
	};
}
