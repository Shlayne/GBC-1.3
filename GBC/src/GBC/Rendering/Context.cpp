#include "Context.h"
#include "gbcpch.h"
#include "GBC/Rendering/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace gbc
{
	Ref<Context> Context::CreateRef(void* nativeContext)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLContext>(nativeContext);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Context> Context::CreateScope(void* nativeContext)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLContext>(nativeContext);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
