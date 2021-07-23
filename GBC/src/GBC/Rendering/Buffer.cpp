#include "gbcpch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLBuffer.h"

namespace gbc
{
	static int GetElementCount(VertexBufferElementType type)
	{
		switch (type)
		{
			case VertexBufferElementType::Int:    return 1;
			case VertexBufferElementType::Int2:   return 2;
			case VertexBufferElementType::Int3:   return 3;
			case VertexBufferElementType::Int4:   return 4;
			case VertexBufferElementType::UInt:   return 1;
			case VertexBufferElementType::UInt2:  return 2;
			case VertexBufferElementType::UInt3:  return 3;
			case VertexBufferElementType::UInt4:  return 4;
			case VertexBufferElementType::Float:  return 1;
			case VertexBufferElementType::Float2: return 2;
			case VertexBufferElementType::Float3: return 3;
			case VertexBufferElementType::Float4: return 4;
		}

		GBC_CORE_ASSERT(false, "Unknown Vertex Buffer Element Type!");
		return 0;
	}

	static int GetElementSize(VertexBufferElementType type)
	{
		return GetElementCount(type) * sizeof(float);
	}

	VertexBufferElement::VertexBufferElement(VertexBufferElementType type, const std::string& name, bool normalized) noexcept
		: type(type), name(name), normalized(normalized), count(GetElementCount(type)), size(GetElementSize(type)) {}

	VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferElement> elements) noexcept
		: elements(elements)
	{
		for (VertexBufferElement& element : this->elements)
		{
			element.offset = stride;
			stride += element.size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::CreateRef(uint32_t size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLVertexBuffer>(size, data, usage);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<VertexBuffer> VertexBuffer::CreateScope(uint32_t size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLVertexBuffer>(size, data, usage);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::CreateRef(uint32_t count, const void* data, BufferUsage usage, IndexBufferElementType type)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLIndexBuffer>(count, data, usage, type);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<IndexBuffer> IndexBuffer::CreateScope(uint32_t count, const void* data, BufferUsage usage, IndexBufferElementType type)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLIndexBuffer>(count, data, usage, type);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
