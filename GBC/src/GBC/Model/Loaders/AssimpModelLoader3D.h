#pragma once

#include "GBC/Model/ModelLoader3D.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace gbc
{
	class AssimpModelLoader3D : public ModelLoader3D
	{
	public:
		virtual Ref<Model3D> LoadModel(const std::filesystem::path& filepath) override;
	private:
		void ProcessNode(Ref<Model3D>& model3D, const aiScene* scene, aiNode* node);
		Mesh3D ProcessMesh(Ref<Model3D>& model3D, const aiScene* scene, aiMesh* mesh);
	};
}
