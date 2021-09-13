#include "gbcpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

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

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GBC_CORE_ASSERT(vertexBuffer != nullptr, "Vertex buffer is nullptr!");

		glBindVertexArray(rendererID);
		vertexBuffer->Bind();
		vertexBuffers.push_back(vertexBuffer);
	
		const VertexBufferLayout& layout = vertexBuffer->GetLayout();
		const std::vector<VertexBufferElement>& elements = layout.GetElements();
		for (uint32_t i = 0; i < static_cast<uint32_t>(elements.size()); i++)
		{
			const VertexBufferElement& element = elements[i];
			glEnableVertexAttribArray(i);
			switch (element.type)
			{
				case VertexBufferElementType::Float:
				case VertexBufferElementType::Float2:
				case VertexBufferElementType::Float3:
				case VertexBufferElementType::Float4:
					glVertexAttribPointer(i, element.count, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)static_cast<size_t>(element.offset));
					break;
				case VertexBufferElementType::Int:
				case VertexBufferElementType::Int2:
				case VertexBufferElementType::Int3:
				case VertexBufferElementType::Int4:
					glVertexAttribIPointer(i, element.count, GL_INT, layout.GetStride(), (const void*)static_cast<size_t>(element.offset));
					break;
				case VertexBufferElementType::UInt:
				case VertexBufferElementType::UInt2:
				case VertexBufferElementType::UInt3:
				case VertexBufferElementType::UInt4:
					glVertexAttribIPointer(i, element.count, GL_UNSIGNED_INT, layout.GetStride(), (const void*)static_cast<size_t>(element.offset));
					break;
			}
		}
	}
}
