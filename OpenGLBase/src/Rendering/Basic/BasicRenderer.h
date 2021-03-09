#pragma once

#include "BasicModel.h"
#include "Rendering/Texture.h"

namespace cbc
{
	class BasicRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& cameraTransform, const glm::mat4& projection);
		static void EndScene();

		static void Submit(const BasicModel& model, const glm::mat4& transform = glm::mat4(1.0f), Ref<Texture> texture = nullptr);
	private:
		static void EnsureBatch(unsigned int vertexCount, unsigned int indexCount, unsigned int texIndex = 0);
		static void Reset();
		static unsigned int GetTextureIndex(const Ref<Texture>& texture);
	};
}
