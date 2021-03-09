#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

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
		default: return 0; // TODO: assert
	}
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
		default: return 0; // TODO: assert
	}
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

Ref<VertexBuffer> VertexBuffer::Create(unsigned int size, const void* data, BufferUsage usage)
{
	return CreateRef<OpenGLVertexBuffer>(size, data, usage);
}

Ref<IndexBuffer> IndexBuffer::Create(unsigned int count, const void* data, BufferUsage usage)
{
	return CreateRef<OpenGLIndexBuffer>(count, data, usage);
}
