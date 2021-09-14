#include "gbcpch.h"
#include "GBC/Rendering/Buffer.h"
#include "GBC/Rendering/RendererAPI.h"
#include "Platform/Renderer/OpenGL/OpenGLBuffer.h"

namespace gbc
{
	static int32_t GetElementCount(VertexBufferElementType type)
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

	static int32_t GetElementSize(VertexBufferElementType type)
	{
		switch (type)
		{
			case VertexBufferElementType::Int:    return 4;
			case VertexBufferElementType::Int2:   return 8;
			case VertexBufferElementType::Int3:   return 12;
			case VertexBufferElementType::Int4:   return 16;
			case VertexBufferElementType::UInt:   return 4;
			case VertexBufferElementType::UInt2:  return 8;
			case VertexBufferElementType::UInt3:  return 12;
			case VertexBufferElementType::UInt4:  return 16;
			case VertexBufferElementType::Float:  return 4;
			case VertexBufferElementType::Float2: return 8;
			case VertexBufferElementType::Float3: return 12;
			case VertexBufferElementType::Float4: return 16;
		}

		GBC_CORE_ASSERT(false, "Unknown Vertex Buffer Element Type!");
		return 0;
	}

	VertexBufferElement::VertexBufferElement(VertexBufferElementType type, const std::string& name, bool normalized) noexcept
		: type(type), name(name), normalized(normalized), count(GetElementCount(type)), size(GetElementSize(type)) {}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size, data, usage);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, const void* data, BufferUsage usage, IndexBufferElementType type)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(count, data, usage, type);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding, const void* data, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::Headless: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding, data, usage);
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
