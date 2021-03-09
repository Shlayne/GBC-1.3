#include "OpenGLTexture.h"
#include <gl/glew.h>

OpenGLTexture::OpenGLTexture(Ref<LocalTexture2D> texture)
	: texture(texture)
{
	// TODO: this should be from texture specifications
	switch (texture->GetChannels())
	{
		case 3: internalFormat = GL_RGB8; format = GL_RGB; break;
		case 4: internalFormat = GL_RGBA8; format = GL_RGBA; break;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
	glTextureStorage2D(rendererID, 1, internalFormat, texture->GetWidth(), texture->GetHeight());

	// TODO: this should be from texture specifications
	glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// TODO: this should be from texture specifications ----------------------------------------VVVVVVVVVVVVVVVV
	glTextureSubImage2D(rendererID, 0, 0, 0, texture->GetWidth(), texture->GetHeight(), format, GL_UNSIGNED_BYTE, texture->GetData());
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &rendererID);
}

void OpenGLTexture::Bind(unsigned int slot) const
{
	glBindTextureUnit(slot, rendererID);
}

void OpenGLTexture::Unbind(unsigned int slot) const
{
	glBindTextureUnit(slot, 0);
}

void OpenGLTexture::Update()
{
	// TODO: this should be from texture specifications ----------------------------------------VVVVVVVVVVVVVVVV
	glTextureSubImage2D(rendererID, 0, 0, 0, texture->GetWidth(), texture->GetHeight(), format, GL_UNSIGNED_BYTE, texture->GetData());
}
