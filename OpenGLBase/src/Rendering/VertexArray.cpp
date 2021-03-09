#include "cbcpch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace cbc
{
	Ref<VertexArray> VertexArray::CreateRef()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateRef<OpenGLVertexArray>();
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<VertexArray> VertexArray::CreateScope()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateScope<OpenGLVertexArray>();
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
