#include "Buffer.h"
#include <gl/glew.h>

static GLenum GetGLUsage(BufferUsage usage)
{
	switch (usage)
	{
		case BufferUsage::StreamDraw:  return GL_STREAM_DRAW;
		case BufferUsage::StreamRead:  return GL_STREAM_READ;
		case BufferUsage::StreamCopy:  return GL_STREAM_COPY;
		case BufferUsage::StaticDraw:  return GL_STATIC_DRAW;
		case BufferUsage::StaticRead:  return GL_STATIC_READ;
		case BufferUsage::StaticCopy:  return GL_STATIC_COPY;
		case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
		case BufferUsage::DynamicRead: return GL_DYNAMIC_READ;
		case BufferUsage::DynamicCopy: return GL_DYNAMIC_COPY;
		default: return 0;
	}
}

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
	: elements(elements)
{
	for (BufferElement& element : this->elements)
	{
		element.offset = stride;
		element.count = static_cast<int>(element.type) & 0x000000ff;
		element.size = 4 * element.count;
		stride += element.size;
	}
}

VertexBuffer::VertexBuffer(unsigned int size, const void* data, BufferUsage usage)
{
	glCreateBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GetGLUsage(usage));
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(unsigned int size, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

IndexBuffer::IndexBuffer(unsigned int count, const void* data, BufferUsage usage)
	: count(count)
{
	glCreateBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GetGLUsage(usage));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(unsigned int count, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data);
}
