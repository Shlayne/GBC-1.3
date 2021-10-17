#include "gbcpch.h"
#include "Model3D.h"
#include "GBC/Rendering/RendererAPI.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gbc
{
	Model3D::Model3D(const Ref<Mesh3D>& mesh, const std::filesystem::path& id)
		: filepath(id), meshes{ mesh } {}

	Model3D::Model3D(const std::filesystem::path& filepath, UUID uuid)
		: filepath(filepath), uuid(uuid)
	{
		Load();
	}

	Ref<Model3D> Model3D::Create(const Ref<Mesh3D>& mesh, const std::filesystem::path& id)
	{
		return CreateRef<Model3D>(mesh, id);
	}

	Ref<Model3D> Model3D::Create(const std::filesystem::path& filepath, UUID uuid)
	{
		auto model3D = CreateRef<Model3D>(filepath, uuid);
		if (model3D->meshes.empty())
			return nullptr;
		return model3D;
	}

	void Model3D::Load()
	{
		Assimp::Importer importer;

		std::string filepathString = filepath.string();
		uint32_t flags = aiProcess_Triangulate;
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			flags |= aiProcess_FlipUVs;
		const aiScene* scene = importer.ReadFile(filepathString, flags);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			return;

		ProcessNode(scene->mRootNode, scene);
	}

	void Model3D::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(CreateRef<Mesh3D>(ProcessMesh(mesh, scene)));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene);
	}

	static constexpr glm::vec3 ToVec3(const aiVector3t<ai_real>& v) noexcept { return { v.x, v.y, v.z }; }
	static constexpr glm::vec2 ToVec2(const aiVector3t<ai_real>& v) noexcept { return { v.x, v.y }; }

	Mesh3D Model3D::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		Mesh3D mesh3D;

		// Process vertices
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			mesh3D.vertices.emplace_back(
				ToVec3(mesh->mVertices[i]),
				mesh->HasNormals() ? ToVec3(mesh->mNormals[i]) : glm::vec3{ 0.0f },
				mesh->HasTextureCoords(0) ? ToVec2(mesh->mTextureCoords[0][i]) : glm::vec3{ 0.0f }
			);
		}

		// Process indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				mesh3D.indices.push_back(face.mIndices[j]);
		}

		// TODO:
		// Process materials
		// 24:14 https://www.youtube.com/watch?v=yQx_pMsYqzU
		//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		return mesh3D;
	}
}
