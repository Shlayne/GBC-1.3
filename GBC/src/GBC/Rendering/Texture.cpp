#include "gbcpch.h"
#include "Texture.h"
#include "GBC/Rendering/RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLTexture.h"

namespace gbc
{
	TextureSpecification& TextureSpecification::operator=(const TextureSpecification& specs)
	{
		if (this != &specs)
		{
			texture = specs.texture;
			minFilter = specs.minFilter;
			magFilter = specs.magFilter;
			wrapS = specs.wrapS;
			wrapT = specs.wrapT;
		}
		return *this;
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture>(specification);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification, const Ref<Framebuffer>& framebuffer, int32_t attachmentIndex)
	{
		GBC_CORE_ASSERT(specification.texture == nullptr, "Texture Specification texture not nullptr when constructing with framebuffer!");

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture>(specification, framebuffer, attachmentIndex);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
