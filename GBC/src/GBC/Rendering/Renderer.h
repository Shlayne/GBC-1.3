#pragma once

#include "GBC/Rendering/RendererAPI.h"
#include "GBC/Rendering/VertexArray.h"

namespace gbc
{
	class Renderer
	{
	private:
		friend class Application;
		static void Init();
		static void Shutdown();
	public:
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void EnableBlending();
		static void DisableBlending();
		static void EnableCullFace();
		static void DisableCullFace();

		static void SetViewport(const glm::ivec2& position, const glm::ivec2& size);
		static void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

		static void Clear();
		static void SetClearColor(const glm::vec4& color);
		static void SetClearColor(float red, float green, float blue, float alpha);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, uint32_t offset = 0, uint32_t count = 0, RendererPrimitive primitive = RendererPrimitive::Triangles);
	private:
		static Scope<RendererAPI> api;
		friend class RendererCapabilities;
	};
}
