#pragma once

#include "GBC/Rendering/Texture.h"

namespace gbc
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const TextureSpecification& specification);
		OpenGLTexture(const TextureSpecification& specification, UUID uuid);
		OpenGLTexture(const Ref<Framebuffer>& framebuffer, int32_t attachmentIndex);
		virtual ~OpenGLTexture();

		virtual void ChangeSampling(const TextureSpecification& specification) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind(uint32_t slot = 0) const override;

		virtual RendererID GetRendererID() const override { return rendererID; }
		virtual const Ref<LocalTexture2D>& GetTexture() const override { return specification.texture; }
		virtual const TextureSpecification& GetSpecification() const override { return specification; }

		virtual UUID GetUUID() const override { return uuid; }
		virtual const std::filesystem::path& GetFilepath() const override { return specification.texture->GetFilepath(); }

		virtual void Update() override;
		virtual bool IsFramebufferTexture() const override { return !ownsRendererID; }
	private:
		void Init();
	private:
		TextureSpecification specification;
		UUID uuid;
		bool ownsRendererID = true;

		RendererID rendererID = 0;
		uint32_t internalFormat = 0;
		uint32_t format = 0;
		uint32_t type = 0;
	};
}
