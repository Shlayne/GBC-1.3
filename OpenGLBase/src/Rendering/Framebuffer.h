#pragma once

#include <vector>
#include <initializer_list>

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
	FramebufferTextureSpecification(FramebufferTextureFormat format, FramebufferFilterMode minFilter, FramebufferFilterMode magFilter, FramebufferWrapMode wrapS, FramebufferWrapMode wrapT)
		: format(format), minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}

	FramebufferTextureFormat format = FramebufferTextureFormat::None;
	FramebufferFilterMode minFilter = FramebufferFilterMode::Linear;
	FramebufferFilterMode magFilter = FramebufferFilterMode::Nearest;
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
	Framebuffer(FramebufferSpecification specification);
	~Framebuffer();

	void Bind();
	void Unbind();

	void OnViewportResize(int width, int height);
	const FramebufferSpecification& GetSpecification() const { return specification; }

	unsigned int GetColorAttachment(unsigned int index = 0) const { return index < colorAttachments.size() ? colorAttachments[index] : 0; }
	unsigned int GetDepthAttachment() const { return depthAttachment; }

	bool GetColorPixel(void* pixel, int x, int y, unsigned int index = 0) const;

	void ClearColorAttachment(const void* value, unsigned int index = 0);
private:
	void Recreate();
	void Clear();

	FramebufferSpecification specification;
	unsigned int rendererID = 0;

	std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
	FramebufferTextureSpecification depthAttachmentSpecification;

	std::vector<unsigned int> colorAttachments;
	unsigned int depthAttachment = 0;
};
