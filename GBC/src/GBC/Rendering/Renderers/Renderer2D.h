#pragma once

#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Texture.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/CircleRendererComponent.h"

namespace gbc
{
	// This renderer is capable of rendering the following:
	//	quads with different translations, rotations, scales, colors, textures, and tiling factors
	//	circles with different translations, scales, thicknesses, and colors
	class Renderer2D
	{
	public:
		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene();
		static void Flush();
	public:
		static void DrawQuad(const glm::vec3& translation, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f));
		static void DrawQuad(const glm::vec3& translation, float rotation, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f));
		static void DrawQuad(const glm::vec3& translation, const glm::vec2& scale, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f));
		static void DrawQuad(const glm::vec3& translation, float rotation, const glm::vec2& scale, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f));

		// Aliases
		static inline void DrawQuad(const glm::vec3& translation, const SpriteRendererComponent& sprite) { DrawQuad(translation, sprite.color, sprite.texture, sprite.tilingFactor); }
		static inline void DrawQuad(const glm::vec3& translation, float rotation, const SpriteRendererComponent& sprite) { DrawQuad(translation, rotation, sprite.color, sprite.texture, sprite.tilingFactor); }
		static inline void DrawQuad(const glm::vec3& translation, const glm::vec2& scale, const SpriteRendererComponent& sprite) { DrawQuad(translation, scale, sprite.color, sprite.texture, sprite.tilingFactor); }
		static inline void DrawQuad(const glm::vec3& translation, float rotation, const glm::vec2& scale, const SpriteRendererComponent& sprite) { DrawQuad(translation, rotation, scale, sprite.color, sprite.texture, sprite.tilingFactor); }
		static inline void DrawQuad(const glm::vec2& translation, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f)) { DrawQuad(glm::vec3(translation, 0.0f), color, texture, tilingFactor); }
		static inline void DrawQuad(const glm::vec2& translation, float rotation, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f)) { DrawQuad(glm::vec3(translation, 0.0f), rotation, color, texture, tilingFactor); }
		static inline void DrawQuad(const glm::vec2& translation, const glm::vec2& scale, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f)) { DrawQuad(glm::vec3(translation, 0.0f), scale, color, texture, tilingFactor); }
		static inline void DrawQuad(const glm::vec2& translation, float rotation, const glm::vec2& scale, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f)) { DrawQuad(glm::vec3(translation, 0.0f), rotation, scale, color, texture, tilingFactor); }
		static inline void DrawQuad(const glm::vec2& translation, const SpriteRendererComponent& sprite) { DrawQuad(glm::vec3(translation, 0.0f), sprite); }
		static inline void DrawQuad(const glm::vec2& translation, float rotation, const SpriteRendererComponent& sprite) { DrawQuad(glm::vec3(translation, 0.0f), rotation, sprite); }
		static inline void DrawQuad(const glm::vec2& translation, const glm::vec2& scale, const SpriteRendererComponent& sprite) { DrawQuad(glm::vec3(translation, 0.0f), scale, sprite); }
		static inline void DrawQuad(const glm::vec2& translation, float rotation, const glm::vec2& scale, const SpriteRendererComponent& sprite) { DrawQuad(glm::vec3(translation, 0.0f), rotation, scale, sprite); }
		static inline void DrawQuad(const glm::mat4& transform, const SpriteRendererComponent& sprite) { DrawQuad(transform, sprite.color, sprite.texture, sprite.tilingFactor); }
		
		// Every DrawQuad eventually feeds to here
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f));
	public:
		static void DrawCircle(const glm::vec3& translation, float thickness, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawCircle(const glm::vec3& translation, const glm::vec2& scale, float thickness, const glm::vec4& color = glm::vec4(1.0f));

		// Aliases
		static inline void DrawCircle(const glm::vec3& translation, const CircleRendererComponent& circle) { DrawCircle(translation, circle.thickness, circle.color); }
		static inline void DrawCircle(const glm::vec3& translation, const glm::vec2& scale, const CircleRendererComponent& circle) { DrawCircle(translation, scale, circle.thickness, circle.color); }
		static inline void DrawCircle(const glm::vec2& translation, float thickness, const glm::vec4& color = glm::vec4(1.0f)) { DrawCircle(glm::vec3(translation, 0.0f), thickness, color); }
		static inline void DrawCircle(const glm::vec2& translation, const glm::vec2& scale, float thickness, const glm::vec4& color = glm::vec4(1.0f)) { DrawCircle(glm::vec3(translation, 0.0f), scale, thickness, color); }
		static inline void DrawCircle(const glm::vec2& translation, const CircleRendererComponent& circle) { DrawCircle(glm::vec3(translation, 0.0f), circle.thickness, circle.color); }
		static inline void DrawCircle(const glm::vec2& translation, const glm::vec2& scale, const CircleRendererComponent& circle) { DrawCircle(glm::vec3(translation, 0.0f), scale, circle.thickness, circle.color); }
		static inline void DrawCircle(const glm::mat4& transform, const CircleRendererComponent& circle) { DrawCircle(transform, circle.thickness, circle.color); }

		// Every DrawCircle eventually feeds to here
		static void DrawCircle(const glm::mat4& transform, float thickness, const glm::vec4& color = glm::vec4(1.0f));
	private:
		static void FlushQuads();
		static void FlushCircles();
		static void ResetQuads();
		static void ResetCircles();

		static uint32_t GetTexIndex(const Ref<Texture2D>& texture);
		static void EnsureQuadBatch(uint32_t texIndex);
		static void EnsureCircleBatch();
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
			constexpr uint32_t GetQuadCount() const noexcept { return quadCount; }
			constexpr uint32_t GetCircleCount() const noexcept { return circleCount; }
			constexpr uint32_t GetTextureCount() const noexcept { return textureCount; }
			constexpr uint32_t GetVertexCount() const noexcept { return (quadCount + circleCount) * 4; }
			constexpr uint32_t GetIndexCount() const noexcept { return (quadCount + circleCount) * 6; }
		private:
			friend class Renderer2D;
			uint32_t drawCallCount = 0;
			uint32_t quadCount = 0;
			uint32_t circleCount = 0;
			uint32_t textureCount = 0;
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
#endif
	};
}
