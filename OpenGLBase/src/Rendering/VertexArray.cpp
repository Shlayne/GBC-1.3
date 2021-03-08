#include "VertexArray.h"

#include <gl/glew.h>

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::bind() const
{
	glBindVertexArray(rendererID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const VertexBuffer* vertexBuffer)
{
	glBindVertexArray(rendererID);
	vertexBuffer->bind();
	vertexBuffers.push_back(vertexBuffer);
	
	const BufferLayout& layout = vertexBuffer->getLayout();
	const std::vector<BufferElement>& elements = layout.getElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const BufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		switch (element.type)
		{
			case ElementType::Float:
			case ElementType::Float2:
			case ElementType::Float3:
			case ElementType::Float4:
				glVertexAttribPointer(i, element.count, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)((size_t)element.offset));
				break;
			case ElementType::Int:
			case ElementType::Int2:
			case ElementType::Int3:
			case ElementType::Int4:
				glVertexAttribIPointer(i, element.count, GL_INT, layout.getStride(), (const void*)((size_t)element.offset));
				break;
		}
	}
}
