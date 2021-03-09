#pragma once

#include <vector>
#include "Buffer.h"

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) = 0;
	virtual const std::vector<const VertexBuffer*>& GetVertexBuffers() = 0;

	static Ref<VertexArray> Create();
};
