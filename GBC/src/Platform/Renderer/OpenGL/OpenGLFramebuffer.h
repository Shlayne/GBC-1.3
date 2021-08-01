#pragma once

#include "GBC/Rendering/Framebuffer.h"

namespace gbc
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void OnViewportResize(int width, int height) override;
		virtual const FramebufferSpecification& GetSpecification() const override { return specification; }

		virtual RendererID GetColorAttachment(uint32_t index = 0) const override { return index < colorAttachments.size() ? colorAttachments[index] : 0; }
		virtual RendererID GetDepthAttachment() const override { return depthAttachment; }

		virtual void GetColorPixel(void* pixel, int x, int y, uint32_t index = 0) const override;

		virtual void ClearColorAttachment(int value, uint32_t index = 0) override;
	private:
		void Recreate();
		void Clear();

		FramebufferSpecification specification;
		RendererID rendererID = 0;

		std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
		FramebufferTextureSpecification depthAttachmentSpecification;

		std::vector<RendererID> colorAttachments;
		RendererID depthAttachment = 0;
	};
}
