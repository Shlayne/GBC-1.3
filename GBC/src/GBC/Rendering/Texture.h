#pragma once

#include "LocalTexture2D.h"
#include "Framebuffer.h"

namespace gbc
{
	enum class TextureFormat
	{
		None = 0,

		RGB8,
		RGBA8
	};

	enum class TextureFilterMode
	{
		Linear,
		Nearest
	};

	enum class TextureWrapMode
	{
		ClampToEdge,
		Repeat
	};

	struct TextureSpecification
	{
		TextureSpecification() = default;
		TextureSpecification(const Ref<LocalTexture2D>& texture)
			: texture(texture) {}
		TextureSpecification(const Ref<LocalTexture2D>& texture, TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapR, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: texture(texture), minFilter(minFilter), magFilter(magFilter), wrapR(wrapR), wrapS(wrapS), wrapT(wrapT) {}
		TextureSpecification(TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapR, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: minFilter(minFilter), magFilter(magFilter), wrapR(wrapR), wrapS(wrapS), wrapT(wrapT) {}

		Ref<LocalTexture2D> texture = nullptr;
		TextureFilterMode minFilter = TextureFilterMode::Linear;
		TextureFilterMode magFilter = TextureFilterMode::Nearest;
		TextureWrapMode wrapR = TextureWrapMode::ClampToEdge;
		TextureWrapMode wrapS = TextureWrapMode::ClampToEdge;
		TextureWrapMode wrapT = TextureWrapMode::ClampToEdge;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual const Ref<LocalTexture2D>& GetTexture() const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;

		// Call this when you want to update the internal
		// texture after the the local texture has updated.
		// Only call if this was not constructed with a Framebuffer.
		virtual void Update() = 0;

		static Ref<Texture> CreateRef(TextureSpecification specification);
		static Scope<Texture> CreateScope(TextureSpecification specification);

		static Ref<Texture> CreateRef(TextureSpecification specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex);
		static Scope<Texture> CreateScope(TextureSpecification specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex);
	};
}
