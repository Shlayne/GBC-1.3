#pragma once

#include "Rendering/RendererAPI.h"

namespace cbc
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

		// TODO: render capabilities
		virtual int GetMaxTextureSlots() override;
		virtual int GetMaxTextureSize() override;
		virtual int GetMaxFramebufferWidth() override;
		virtual int GetMaxFramebufferHeight() override;
		virtual int GetMaxFramebufferColorAttachments() override;

		virtual void SetViewport(int x, int y, int width, int height) override;

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, unsigned int count) override;
	};
}
