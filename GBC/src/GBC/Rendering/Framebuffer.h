#pragma once

#include "GBC/Core/Core.h"
#include <vector>

namespace gbc
{
	enum class FramebufferTextureFormat : uint8_t
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

	enum class FramebufferFilterMode : uint8_t
	{
		Linear,
		Nearest
	};

	enum class FramebufferWrapMode : uint8_t
	{
		ClampToEdge,
		Repeat
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: format(format) {}
		FramebufferTextureSpecification(FramebufferTextureFormat format, FramebufferFilterMode minFilter, FramebufferFilterMode magFilter, FramebufferWrapMode wrapS, FramebufferWrapMode wrapT)
			: format(format), minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}

		FramebufferTextureFormat format = FramebufferTextureFormat::None;
		FramebufferFilterMode minFilter = FramebufferFilterMode::Linear;
		FramebufferFilterMode magFilter = FramebufferFilterMode::Linear;
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
		int32_t width = 0;
		int32_t height = 0;
		int32_t samples = 1;
		FramebufferAttachmentSpecification attachments;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void OnViewportResize(int32_t width, int32_t height) = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual RendererID GetColorAttachment(uint32_t index = 0) const = 0;
		virtual RendererID GetDepthAttachment() const = 0;

		virtual int32_t GetColorPixel(int32_t x, int32_t y, uint32_t index = 0) const = 0;

		virtual void ClearColorAttachment(int32_t value, uint32_t index = 0) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specification); 
	};
}
