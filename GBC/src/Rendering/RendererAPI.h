#pragma once

#include "VertexArray.h"

namespace gbc
{
	class RendererAPI
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		virtual void EnableBlending() = 0;
		virtual void DisableBlending() = 0;
		virtual void EnableCullFace() = 0;
		virtual void DisableCullFace() = 0;

		// TODO: render capabilities
		virtual int GetMaxTextureSlots() = 0;
		virtual int GetMaxTextureSize() = 0;
		virtual int GetMaxFramebufferWidth() = 0;
		virtual int GetMaxFramebufferHeight() = 0;
		virtual int GetMaxFramebufferColorAttachments() = 0;

		virtual void SetViewport(int x, int y, int width, int height) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, unsigned int count = 0) = 0;

		enum class API { None, OpenGL, Vulkan, Direct3D, Metal };
		inline static API GetAPI() { return api; }
	private:
		static API api;

		static Scope<RendererAPI> CreateScope();
		friend class Renderer;
	};
}
