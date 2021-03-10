#include "gbcpch.h"
#include "RendererAPI.h"
#include "GBC/Platform/OpenGL/OpenGLRendererAPI.h"

namespace gbc
{
	// TODO: this should not be hardcoded here
	RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::CreateScope()
	{
		switch (api)
		{
			case API::None:   return nullptr;
			case API::OpenGL: return gbc::CreateScope<OpenGLRendererAPI>();
		}

		GBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
