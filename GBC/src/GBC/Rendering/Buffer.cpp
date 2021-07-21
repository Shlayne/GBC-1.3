#include "gbcpch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLBuffer.h"

namespace gbc
{
	static int GetElementCount(BufferElementType type)
	{
		switch (type)
		{
			case BufferElementType::Int:    return 1;
			case BufferElementType::Int2:   return 2;
			case BufferElementType::Int3:   return 3;
			case BufferElementType::Int4:   return 4;
			case BufferElementType::UInt:   return 1;
			case BufferElementType::UInt2:  return 2;
			case BufferElementType::UInt3:  return 3;
			case BufferElementType::UInt4:  return 4;
			case BufferElementType::Float:  return 1;
			case BufferElementType::Float2: return 2;
			case BufferElementType::Float3: return 3;
			case BufferElementType::Float4: return 4;
		}

		GBC_CORE_ASSERT(false, "Unknown BufferElementType!");
		return 0;
	}

	static int GetElementSize(BufferElementType type)
	{
		switch (type)
		{
			case BufferElementType::Int:    return 4 * 1;
			case BufferElementType::Int2:   return 4 * 2;
			case BufferElementType::Int3:   return 4 * 3;
			case BufferElementType::Int4:   return 4 * 4;
			case BufferElementType::UInt:   return 4 * 1;
			case BufferElementType::UInt2:  return 4 * 2;
			case BufferElementType::UInt3:  return 4 * 3;
			case BufferElementType::UInt4:  return 4 * 4;
			case BufferElementType::Float:  return 4 * 1;
			case BufferElementType::Float2: return 4 * 2;
			case BufferElementType::Float3: return 4 * 3;
			case BufferElementType::Float4: return 4 * 4;
		}

		GBC_CORE_ASSERT(false, "Unknown BufferElementType!");
		return 0;
	}

	BufferElement::BufferElement(BufferElementType type, const std::string& name, bool normalized) noexcept
		: type(type), name(name), normalized(normalized), count(GetElementCount(type)), size(GetElementSize(type)) {}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) noexcept
		: elements(elements)
	{
		for (BufferElement& element : this->elements)
		{
			element.offset = stride;
			stride += element.size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::CreateRef(uint32_t size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLVertexBuffer>(size, data, usage);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<VertexBuffer> VertexBuffer::CreateScope(uint32_t size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLVertexBuffer>(size, data, usage);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::CreateRef(uint32_t count, const void* data, BufferUsage usage, BufferIndexType type)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateRef<OpenGLIndexBuffer>(count, data, usage, type);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<IndexBuffer> IndexBuffer::CreateScope(uint32_t count, const void* data, BufferUsage usage, BufferIndexType type)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return ::gbc::CreateScope<OpenGLIndexBuffer>(count, data, usage, type);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
