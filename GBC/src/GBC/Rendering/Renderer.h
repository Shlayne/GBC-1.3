#pragma once

#include "VertexArray.h"
#include "RendererAPI.h"

namespace gbc
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void EnableDepthTest();
		static void DisableDepthTest();
		static void EnableBlending();
		static void DisableBlending();
		static void EnableCullFace();
		static void DisableCullFace();

		// TODO: render capabilities
		static int GetMaxTextureSlots();
		static int GetMaxTextureSize();
		static int GetMaxFramebufferWidth();
		static int GetMaxFramebufferHeight();
		static int GetMaxFramebufferColorAttachments();

		static void SetViewport(int x, int y, int width, int height);

		static void Clear();
		static void SetClearColor(const glm::vec4& color);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, unsigned int count = 0);
	private:
		static Scope<RendererAPI> api;
	};
}
