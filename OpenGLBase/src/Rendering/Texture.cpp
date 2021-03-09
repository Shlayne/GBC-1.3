#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

Ref<Texture> Texture::Create(Ref<LocalTexture2D> texture)
{
	return CreateRef<OpenGLTexture>(texture);
}
