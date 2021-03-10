#include "gbcpch.h"
#include "Framebuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace gbc
{
	Ref<Framebuffer> Framebuffer::CreateRef(FramebufferSpecification specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateRef<OpenGLFramebuffer>(specification);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Framebuffer> Framebuffer::CreateScope(FramebufferSpecification specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateScope<OpenGLFramebuffer>(specification);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
