#include "gbcpch.h"
#include "OpenGLTexture.h"
#include "glad/glad.h"

namespace gbc
{
	static GLenum GetOpenGLFilterMode(TextureFilterMode filter)
	{
		switch (filter)
		{
			case TextureFilterMode::Linear:  return GL_LINEAR;
			case TextureFilterMode::Nearest: return GL_NEAREST;
		}

		GBC_CORE_ASSERT(false, "Unknown Texture Filter Mode!");
		return 0;
	}

	static GLenum GetOpenGLWrapMode(TextureWrapMode wrap)
	{
		switch (wrap)
		{
			case TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case TextureWrapMode::Repeat:      return GL_REPEAT;
		}

		GBC_CORE_ASSERT(false, "Unknown Texture Wrap Mode!");
		return 0;
	}

	TextureFormat GetTextureFormat(int channels)
	{
		switch (channels)
		{
			case 3: return TextureFormat::RGB8;
			case 4: return TextureFormat::RGBA8;
		}

		GBC_CORE_ASSERT(false, "Unconvertable number of channels!");
		return TextureFormat::None;
	}

	void GetOpenGLTypes(TextureFormat inFormat, GLenum& internalFormat, GLenum& format, GLenum& type)
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

	OpenGLTexture::OpenGLTexture(TextureSpecification specification)
		: specification(specification)
	{
		if (specification.texture != nullptr && *specification.texture)
		{
			GetOpenGLTypes(GetTextureFormat(specification.texture->GetChannels()), internalFormat, format, type);

			glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
			glTextureStorage2D(rendererID, 1, internalFormat, specification.texture->GetWidth(), specification.texture->GetHeight());

			glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(specification.minFilter));
			glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(specification.magFilter));
			glTextureParameteri(rendererID, GL_TEXTURE_WRAP_R, GetOpenGLWrapMode(specification.wrapR));
			glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(specification.wrapS));
			glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(specification.wrapT));

			glTextureSubImage2D(rendererID, 0, 0, 0, specification.texture->GetWidth(), specification.texture->GetHeight(), format, type, specification.texture->GetData());
		}
	}

	OpenGLTexture::OpenGLTexture(TextureSpecification specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex)
		: specification(specification), ownsRendererID(false), rendererID(framebuffer->GetColorAttachment(attachmentIndex)) {}

	OpenGLTexture::~OpenGLTexture()
	{
		if (ownsRendererID)
			glDeleteTextures(1, &rendererID);
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
		glTextureSubImage2D(rendererID, 0, 0, 0, specification.texture->GetWidth(), specification.texture->GetHeight(), format, type, specification.texture->GetData());
	}
}
