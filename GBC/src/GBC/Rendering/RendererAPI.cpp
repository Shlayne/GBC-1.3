#include "gbcpch.h"
#include "RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace gbc
{
	RendererAPI::API RendererAPI::api = RendererAPI::API::Unset;

	void RendererAPI::SetAPI(API api)
	{
		if (RendererAPI::api == API::Unset)
			RendererAPI::api = api;
		else
			GBC_CORE_ASSERT(false, "Attempted to set the renderer API in the middle of execution.");
	}

	Scope<RendererAPI> RendererAPI::CreateScope()
	{
		switch (api)
		{
			case API::None:   return nullptr;
			case API::OpenGL: return ::gbc::CreateScope<OpenGLRendererAPI>();
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
