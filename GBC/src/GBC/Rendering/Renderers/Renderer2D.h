#pragma once

#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Texture.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"

namespace gbc
{
	class Renderer2D
	{
	public:
		static void BeginScene(const Camera& camera, const glm::mat4& view);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
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
		
		// Everything eventually feeds to here
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr, const glm::vec2& tilingFactor = glm::vec2(1.0f));
	private:
		static void Reset();

		static uint32_t GetTexIndex(const Ref<Texture2D>& texture);
		static void EnsureBatch(uint32_t texIndex);
	private:
		friend class Renderer;
		static void Init();
		static void Shutdown();
	public:
#if GBC_ENABLE_STATS
		struct Statistics
		{
		public:
			inline uint32_t GetDrawCallCount() const noexcept { return drawCallCount; }
			inline uint32_t GetQuadCount() const noexcept { return quadCount; }
			inline uint32_t GetTextureCount() const noexcept { return textureCount; }
			inline uint32_t GetVertexCount() const noexcept { return quadCount * 4; }
			inline uint32_t GetIndexCount() const noexcept { return quadCount * 6; }
		private:
			friend class Renderer2D;
			uint32_t drawCallCount = 0;
			uint32_t quadCount = 0;
			uint32_t textureCount = 0;
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
#endif
	};
}
