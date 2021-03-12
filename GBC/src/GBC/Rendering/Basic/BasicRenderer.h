#pragma once

#include "BasicModel.h"
#include "BasicRenderable.h"
#include "glm/glm.hpp"

namespace gbc
{
	class BasicRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& cameraTransform, const glm::mat4& projection);
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
