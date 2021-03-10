#include "gbcpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace gbc
{
	RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::CreateScope()
	{
		switch (api)
		{
			case API::None:   return nullptr;
			case API::OpenGL: return gbc::CreateScope<OpenGLRendererAPI>();
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
