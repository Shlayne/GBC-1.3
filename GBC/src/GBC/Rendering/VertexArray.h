#pragma once

#include "Buffer.h"

namespace gbc
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

		static Ref<VertexArray> CreateRef();
		static Scope<VertexArray> CreateScope();
	};
}
