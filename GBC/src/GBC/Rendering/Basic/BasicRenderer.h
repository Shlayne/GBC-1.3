#pragma once

#include "glm/glm.hpp"
#include "BasicModel.h"
#include "BasicRenderable.h"
#include "GBC/Rendering/Camera.h"

namespace gbc
{
	class BasicRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void Submit(const BasicModel& model, const glm::mat4& transform = glm::mat4(1.0f), const BasicRenderable& renderable = {});

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

		static unsigned int GetTexIndex(const BasicRenderable& renderable);
	};
}
