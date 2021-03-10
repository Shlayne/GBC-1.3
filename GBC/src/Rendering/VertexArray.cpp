#include "gbcpch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace gbc
{
	Ref<VertexArray> VertexArray::CreateRef()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateRef<OpenGLVertexArray>();
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<VertexArray> VertexArray::CreateScope()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateScope<OpenGLVertexArray>();
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
