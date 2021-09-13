#pragma once

#include "GBC/Rendering/Texture.h"
#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Basic/BasicMesh.h"
#include "GBC/Scene/Components/RenderableComponent.h"

namespace gbc
{
	class BasicRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& view);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Submit(const Ref<BasicMesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f), const RenderableComponent& renderableComponent = {});

#if GBC_ENABLE_STATS
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t indexCount = 0;
			uint32_t vertexCount = 0;
			uint32_t textureCount = 0;
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
#endif
	private:
		static void EnsureBatch(uint32_t vertexCount, uint32_t indexCount, uint32_t texIndex = 0);
		static void Reset();

		static uint32_t GetTexIndex(const Ref<Texture2D>& texture);
	};
}
