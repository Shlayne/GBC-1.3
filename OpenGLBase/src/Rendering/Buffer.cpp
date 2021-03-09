#include "Buffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace cbc
{
	static int GetElementCount(BufferElementType type)
	{
		switch (type)
		{
			case BufferElementType::Int:   	return 1;
			case BufferElementType::Int2:  	return 2;
			case BufferElementType::Int3:  	return 3;
			case BufferElementType::Int4:  	return 4;
			case BufferElementType::Float:  return 1;
			case BufferElementType::Float2: return 2;
			case BufferElementType::Float3: return 3;
			case BufferElementType::Float4: return 4;
		}

		CBC_ASSERT(false, "Unknown BufferElementType!");
		return 0;
	}

	static int GetElementSize(BufferElementType type)
	{
		switch (type)
		{
			case BufferElementType::Int:   	return 4 * 1;
			case BufferElementType::Int2:  	return 4 * 2;
			case BufferElementType::Int3:  	return 4 * 3;
			case BufferElementType::Int4:  	return 4 * 4;
			case BufferElementType::Float:  return 4 * 1;
			case BufferElementType::Float2: return 4 * 2;
			case BufferElementType::Float3: return 4 * 3;
			case BufferElementType::Float4: return 4 * 4;
		}

		CBC_ASSERT(false, "Unknown BufferElementType!");
		return 0;
	}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: elements(elements)
	{
		for (BufferElement& element : this->elements)
		{
			element.offset = stride;
			element.count = GetElementCount(element.type);
			element.size = GetElementSize(element.type);
			stride += element.size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::CreateRef(unsigned int size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateRef<OpenGLVertexBuffer>(size, data, usage);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<VertexBuffer> VertexBuffer::CreateScope(unsigned int size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateScope<OpenGLVertexBuffer>(size, data, usage);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::CreateRef(unsigned int count, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateRef<OpenGLIndexBuffer>(count, data, usage);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<IndexBuffer> IndexBuffer::CreateScope(unsigned int count, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateScope<OpenGLIndexBuffer>(count, data, usage);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
