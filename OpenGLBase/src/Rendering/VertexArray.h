#pragma once

#include <vector>
#include "Buffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const VertexBuffer* vertexBuffer);
	inline const std::vector<const VertexBuffer*>& GetVertexBuffers() { return vertexBuffers; }
private:
	unsigned int rendererID = 0;
	std::vector<const VertexBuffer*> vertexBuffers;
};
