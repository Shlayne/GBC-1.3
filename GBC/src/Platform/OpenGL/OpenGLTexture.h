#pragma once

#include "GBC/Rendering/Texture.h"

namespace gbc
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(TextureSpecification specification);
		virtual ~OpenGLTexture();

		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind(unsigned int slot = 0) const override;

		virtual const Ref<LocalTexture2D>& GetTexture() const override { return specification.texture; }
		virtual const TextureSpecification& GetSpecification() const override { return specification; }

		virtual void Update() override;
	private:
		TextureSpecification specification;

		RendererID rendererID = 0;
		unsigned int internalFormat = 0;
		unsigned int format = 0;
		unsigned int type = 0;
	};
}
