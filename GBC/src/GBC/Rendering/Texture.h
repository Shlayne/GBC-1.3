#pragma once

#include "LocalTexture2D.h"

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
		TextureSpecification(Ref<LocalTexture2D> texture)
			: texture(texture) {}
		TextureSpecification(Ref<LocalTexture2D> texture, TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: texture(texture), minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}

		Ref<LocalTexture2D> texture;
		TextureFilterMode minFilter = TextureFilterMode::Linear;
		TextureFilterMode magFilter = TextureFilterMode::Nearest;
		TextureWrapMode wrapS = TextureWrapMode::ClampToEdge;
		TextureWrapMode wrapT = TextureWrapMode::ClampToEdge;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind(unsigned int slot = 0) const = 0;

		virtual const Ref<LocalTexture2D>& GetTexture() const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;

		// Call this when you want to update the internal
		// texture after the the local texture has updated.
		virtual void Update() = 0;

		static Ref<Texture> CreateRef(TextureSpecification specification);
		static Scope<Texture> CreateScope(TextureSpecification specification);
	};
}
