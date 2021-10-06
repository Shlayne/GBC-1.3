#pragma once

#include "GBC/Core/UUID.h"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Rendering/LocalTexture2D.h"

namespace gbc
{
	enum class TextureFormat : uint8_t
	{
		None = 0,

		RGB8,
		RGBA8
	};

	enum class TextureFilterMode : uint8_t
	{
		Linear,
		Nearest
	};

	enum class TextureWrapMode : uint8_t
	{
		ClampToEdge,
		Repeat
	};

	struct TextureSpecification
	{
		TextureSpecification() = default;
		TextureSpecification(const Ref<LocalTexture2D>& texture)
			: texture(texture) {}
		TextureSpecification(const Ref<LocalTexture2D>& texture, TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: texture(texture), minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}
		TextureSpecification(TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}
		TextureSpecification(const TextureSpecification& specs)
			: texture(specs.texture), minFilter(specs.minFilter), magFilter(specs.magFilter), wrapS(specs.wrapS), wrapT(specs.wrapT) {}
		TextureSpecification& operator=(const TextureSpecification& specs);

		Ref<LocalTexture2D> texture = nullptr;
		TextureFilterMode minFilter = TextureFilterMode::Linear;
		TextureFilterMode magFilter = TextureFilterMode::Linear;
		TextureWrapMode wrapS = TextureWrapMode::ClampToEdge;
		TextureWrapMode wrapT = TextureWrapMode::ClampToEdge;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void ChangeSampling(const TextureSpecification& specification) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual RendererID GetRendererID() const = 0;
		virtual const Ref<LocalTexture2D>& GetTexture() const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;
		virtual UUID GetUUID() const = 0;

		// Call this when you want to update the internal
		// texture after the the local texture has updated.
		// Only call if this was not constructed with a Framebuffer.
		virtual void Update() = 0;

		// Returns true if this texture was constructed with a Framebuffer, false otherwise.
		virtual bool IsFramebufferTexture() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const TextureSpecification& specification, UUID uuid);
		static Ref<Texture2D> Create(const Ref<Framebuffer>& framebuffer, int32_t attachmentIndex);
	};
}
