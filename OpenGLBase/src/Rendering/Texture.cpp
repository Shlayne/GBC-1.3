#include "cbcpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace cbc
{
	Ref<Texture> Texture::CreateRef(TextureSpecification specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateRef<OpenGLTexture>(specification);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Texture> Texture::CreateScope(TextureSpecification specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateScope<OpenGLTexture>(specification);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
