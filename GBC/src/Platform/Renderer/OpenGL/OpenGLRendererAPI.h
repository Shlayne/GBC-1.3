#pragma once

#include "GBC/Rendering/RendererAPI.h"

namespace gbc
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;
		virtual void EnableBlending() override;
		virtual void DisableBlending() override;
		virtual void EnableCullFace() override;
		virtual void DisableCullFace() override;

		virtual void SetViewport(int x, int y, int width, int height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, uint32_t offset, uint32_t count, RendererPrimitive primitive) override;

		// Render capabilities
		virtual int GetMaxTextureSlots() override;
		virtual int GetMaxTextureSize() override;
		virtual int GetMaxFramebufferWidth() override;
		virtual int GetMaxFramebufferHeight() override;
		virtual int GetMaxFramebufferColorAttachments() override;
	};
}
