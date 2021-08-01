#include "gbcpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLTexture.h"

namespace gbc
{
	Ref<Texture> Texture::CreateRef(const TextureSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLTexture>(specification);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Texture> Texture::CreateScope(const TextureSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLTexture>(specification);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<Texture> Texture::CreateRef(const TextureSpecification& specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex)
	{
		GBC_CORE_ASSERT(specification.texture == nullptr, "Texture Specification texture not nullptr when constructing with framebuffer!");

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLTexture>(specification, framebuffer, attachmentIndex);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Texture> Texture::CreateScope(const TextureSpecification& specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex)
	{
		GBC_CORE_ASSERT(specification.texture == nullptr, "Texture Specification texture not nullptr when constructing with framebuffer!");

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLTexture>(specification, framebuffer, attachmentIndex);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
