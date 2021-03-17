#pragma once

#include "glm/glm.hpp"
#include "BasicMesh.h"
#include "GBC/Scene/Components/RenderableComponent.h"
#include "GBC/Rendering/Texture.h"
#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/EditorCamera.h"

namespace gbc
{
	class BasicRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Submit(const Ref<BasicMesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f), const RenderableComponent& renderableComponent = {});

#if GBC_ENABLE_STATS
		struct Statistics
		{
			unsigned int drawCalls = 0;
			unsigned int indexCount = 0;
			unsigned int vertexCount = 0;
			unsigned int textureCount = 0;
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
#endif
	private:
		static void EnsureBatch(unsigned int vertexCount, unsigned int indexCount, unsigned int texIndex = 0);
		static void Reset();

		static unsigned int GetTexIndex(const Ref<Texture>& texture);
	};
}
