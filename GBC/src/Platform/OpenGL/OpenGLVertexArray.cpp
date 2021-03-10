#include "gbcpch.h"
#include "OpenGLVertexArray.h"
#include "gl/glew.h"

namespace gbc
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &rendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(rendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		glBindVertexArray(rendererID);
		vertexBuffer->Bind();
		vertexBuffers.push_back(vertexBuffer);
	
		const BufferLayout& layout = vertexBuffer->GetLayout();
		const std::vector<BufferElement>& elements = layout.GetElements();
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const BufferElement& element = elements[i];
			glEnableVertexAttribArray(i);
			switch (element.type)
			{
				case BufferElementType::Float:
				case BufferElementType::Float2:
				case BufferElementType::Float3:
				case BufferElementType::Float4:
					glVertexAttribPointer(i, element.count, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)((size_t)element.offset));
					break;
				case BufferElementType::Int:
				case BufferElementType::Int2:
				case BufferElementType::Int3:
				case BufferElementType::Int4:
					glVertexAttribIPointer(i, element.count, GL_INT, layout.GetStride(), (const void*)((size_t)element.offset));
					break;
			}
		}
	}
}
