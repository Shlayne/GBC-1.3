#include "gbcpch.h"
#include "VertexArray.h"
#include "GBC/Rendering/RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLVertexArray.h"

namespace gbc
{
	Ref<VertexArray> VertexArray::CreateRef()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLVertexArray>();
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<VertexArray> VertexArray::CreateScope()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLVertexArray>();
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
