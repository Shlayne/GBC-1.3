#pragma once

#include "GBC/Model/Model3D.h"
#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	class Renderer3D
	{
	public:
		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene();
	public:
		static void Submit(const Ref<Model3D>& model, const glm::mat4& transform);
	private:
		static void Submit(const Mesh3D& mesh, const glm::mat4& transform, const Ref<Texture2D>& texture);
	private:
		static void Reset();

		static uint32_t GetTexIndex(const Ref<Texture2D>& texture);
		static void EnsureBatch(uint32_t vertexCount, uint32_t indexCount, uint32_t texIndex);
	private:
		friend class Renderer;
		static void Init();
		static void Shutdown();
#if GBC_ENABLE_STATS
	public:
		struct Statistics
		{
		public:
			constexpr uint32_t GetDrawCallCount() const noexcept { return drawCallCount; }
			constexpr uint32_t GetVertexCount() const noexcept { return vertexCount; }
			constexpr uint32_t GetIndexCount() const noexcept { return indexCount; }
			constexpr uint32_t GetMeshCount() const noexcept { return meshCount; }
			constexpr uint32_t GetTextureCount() const noexcept { return textureCount; }
		private:
			friend class Renderer3D;
			uint32_t drawCallCount = 0;
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;
			uint32_t meshCount = 0;
			uint32_t textureCount = 0;
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
#endif
	};
}
