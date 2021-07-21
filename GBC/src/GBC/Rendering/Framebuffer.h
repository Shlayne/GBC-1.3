#pragma once

namespace gbc
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RedInteger,

		// Depth/Stencil
		Depth24Stencil8,

		// Defaults
		Color = RGBA8,
		Depth = Depth24Stencil8
	};

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
			case FramebufferTextureFormat::Depth24Stencil8:
				return true;
			case FramebufferTextureFormat::RGBA8:
			case FramebufferTextureFormat::RedInteger:
				return false;
		}

		GBC_CORE_ASSERT(false, "Unknown Framebuffer Texture Format!");
		return false;
	}

	enum class FramebufferFilterMode
	{
		Linear,
		Nearest
	};

	enum class FramebufferWrapMode
	{
		ClampToEdge,
		Repeat
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: format(format) {}
		FramebufferTextureSpecification(FramebufferTextureFormat format, FramebufferFilterMode minFilter, FramebufferFilterMode magFilter, FramebufferWrapMode wrapR, FramebufferWrapMode wrapS, FramebufferWrapMode wrapT)
			: format(format), minFilter(minFilter), magFilter(magFilter), wrapR(wrapR), wrapS(wrapS), wrapT(wrapT) {}

		FramebufferTextureFormat format = FramebufferTextureFormat::None;
		FramebufferFilterMode minFilter = FramebufferFilterMode::Linear;
		FramebufferFilterMode magFilter = FramebufferFilterMode::Nearest;
		FramebufferWrapMode wrapR = FramebufferWrapMode::ClampToEdge;
		FramebufferWrapMode wrapS = FramebufferWrapMode::ClampToEdge;
		FramebufferWrapMode wrapT = FramebufferWrapMode::ClampToEdge;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification
	{
		int width = 0;
		int height = 0;
		int samples = 1;
		FramebufferAttachmentSpecification attachments;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void OnViewportResize(int width, int height) = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual RendererID GetColorAttachment(uint32_t index = 0) const = 0;
		virtual RendererID GetDepthAttachment() const = 0;

		virtual void GetColorPixel(void* pixel, int x, int y, uint32_t index = 0) const = 0;

		virtual void ClearColorAttachment(int value, uint32_t index = 0) = 0;

		static Ref<Framebuffer> CreateRef(FramebufferSpecification specification); 
		static Scope<Framebuffer> CreateScope(FramebufferSpecification specification);
	};
}