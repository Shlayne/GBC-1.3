#include "gbcpch.h"
#include "RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace gbc
{
	RendererAPI::API RendererAPI::api = static_cast<RendererAPI::API>(GBC_RENDERER_API_ID);

	Scope<RendererAPI> RendererAPI::CreateScope()
	{
		switch (api)
		{
			case API::Headless: return nullptr;
			case API::OpenGL: return ::gbc::CreateScope<OpenGLRendererAPI>();
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
