#include "gbcpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "GBC/Platform/OpenGL/OpenGLTexture.h"

namespace gbc
{
	Ref<Texture> Texture::CreateRef(TextureSpecification specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateRef<OpenGLTexture>(specification);
		}

		GBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Texture> Texture::CreateScope(TextureSpecification specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateScope<OpenGLTexture>(specification);
		}

		GBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
