#pragma once

#include "GBC/Rendering/Texture.h"

namespace gbc
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const TextureSpecification& specification);
		OpenGLTexture(const TextureSpecification& specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex);
		virtual ~OpenGLTexture();

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind(uint32_t slot = 0) const override;

		virtual const Ref<LocalTexture2D>& GetTexture() const override { return specification.texture; }
		virtual const TextureSpecification& GetSpecification() const override { return specification; }

		virtual void Update() override;
	private:
		TextureSpecification specification;
		bool ownsRendererID = true;

		RendererID rendererID = 0;
		uint32_t internalFormat = 0;
		uint32_t format = 0;
		uint32_t type = 0;
	};
}
