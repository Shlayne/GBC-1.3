#include "gbcpch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>

namespace gbc
{
	static constexpr GLenum GetOpenGLFilterMode(TextureFilterMode filter)
	{
		switch (filter)
		{
			case TextureFilterMode::Linear:  return GL_LINEAR;
			case TextureFilterMode::Nearest: return GL_NEAREST;
		}

		GBC_CORE_ASSERT(false, "Unknown Texture Filter Mode!");
		return 0;
	}

	static constexpr GLenum GetOpenGLWrapMode(TextureWrapMode wrap)
	{
		switch (wrap)
		{
			case TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case TextureWrapMode::Repeat:      return GL_REPEAT;
		}

		GBC_CORE_ASSERT(false, "Unknown Texture Wrap Mode!");
		return 0;
	}

	static constexpr TextureFormat GetTextureFormat(int32_t channels)
	{
		switch (channels)
		{
			case 3: return TextureFormat::RGB8;
			case 4: return TextureFormat::RGBA8;
		}

		GBC_CORE_ASSERT(false, "Unconvertable number of channels!");
		return TextureFormat::None;
	}

	static constexpr void GetOpenGLTypes(TextureFormat inFormat, GLenum& internalFormat, GLenum& format, GLenum& type)
	{
		switch (inFormat)
		{
			case TextureFormat::RGB8:
				internalFormat = GL_RGB8;
				format = GL_RGB;
				type = GL_UNSIGNED_BYTE;
				return;
			case TextureFormat::RGBA8:
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
				type = GL_UNSIGNED_BYTE;
				return;
		}

		GBC_CORE_ASSERT(false, "Unknown Texture Format!");
	}

	OpenGLTexture::OpenGLTexture(const TextureSpecification& specification)
		: specification(specification)
	{
		Init();
	}

	OpenGLTexture::OpenGLTexture(const TextureSpecification& specification, UUID uuid)
		: specification(specification), uuid(uuid)
	{
		GBC_CORE_ASSERT(!specification.texture->GetFilepath().empty(), "Texture with UUID must be created from a file!");
		Init();
	}

	void OpenGLTexture::Init()
	{
		GBC_CORE_ASSERT(specification.texture != nullptr && *specification.texture, "Specifications texture is nullptr!");

		GetOpenGLTypes(GetTextureFormat(specification.texture->GetChannels()), internalFormat, format, type);

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, specification.texture->GetWidth(), specification.texture->GetHeight());

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(specification.minFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(specification.magFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(specification.wrapS));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(specification.wrapT));

		glTextureSubImage2D(rendererID, 0, 0, 0, specification.texture->GetWidth(), specification.texture->GetHeight(), format, type, specification.texture->GetData());
	}

	OpenGLTexture::OpenGLTexture(const Ref<Framebuffer>& framebuffer, int32_t attachmentIndex)
		: ownsRendererID(false), rendererID(framebuffer->GetColorAttachment(attachmentIndex)),
		// UUID not needed for this type of texture because it is simply to render a framebuffer attachment
		uuid(0) {}

	OpenGLTexture::~OpenGLTexture()
	{
		if (ownsRendererID)
			glDeleteTextures(1, &rendererID);
	}

	void OpenGLTexture::ChangeSampling(const TextureSpecification& specification)
	{
		GBC_CORE_ASSERT(ownsRendererID, "You can't recreate a texture associated with a framebuffer!");

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(specification.minFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(specification.magFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(specification.wrapS));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(specification.wrapT));

		this->specification.minFilter = specification.minFilter;
		this->specification.magFilter = specification.magFilter;
		this->specification.wrapS = specification.wrapS;
		this->specification.wrapT = specification.wrapT;
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, rendererID);
	}

	void OpenGLTexture::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}

	void OpenGLTexture::Update()
	{
		GBC_CORE_ASSERT(ownsRendererID, "A Texture2D must have a local texture to update!");
		glTextureSubImage2D(rendererID, 0, 0, 0, specification.texture->GetWidth(), specification.texture->GetHeight(), format, type, specification.texture->GetData());
	}
}
