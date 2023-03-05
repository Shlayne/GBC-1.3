#include "gbcpch.h"
#include "AssimpModelLoader3D.h"
#include "GBC/Rendering/RendererAPI.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gbc
{
	Ref<Model3D> AssimpModelLoader3D::LoadModel(const std::filesystem::path& filepath)
	{
		Assimp::Importer importer;

		std::string filepathString = filepath.string();
		uint32_t flags = aiProcess_Triangulate;
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			flags |= aiProcess_FlipUVs;
		const aiScene* scene = importer.ReadFile(filepathString, flags);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			return nullptr;

		Ref<Model3D> model3D;
		ProcessNode(model3D, scene, scene->mRootNode);
		return model3D;
	}

	void AssimpModelLoader3D::ProcessNode(Ref<Model3D>& model3D, const aiScene* scene, aiNode* node)
	{
		auto& meshes = model3D->GetMeshes();

		// Process this node's mesh
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(model3D, scene, mesh));
		}

		// Process this node's children's meshes
		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessNode(model3D, scene, node->mChildren[i]);
	}

	static constexpr glm::vec3 ToVec3(const aiVector3t<ai_real>& v) noexcept { return { v.x, v.y, v.z }; }
	static constexpr glm::vec2 ToVec2(const aiVector3t<ai_real>& v) noexcept { return { v.x, v.y }; }

	Mesh3D AssimpModelLoader3D::ProcessMesh(Ref<Model3D>& model3D, const aiScene* scene, aiMesh* mesh)
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
