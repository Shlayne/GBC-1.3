#pragma once

#include "Rendering/VertexArray.h"

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) override;
	virtual const std::vector<const VertexBuffer*>& GetVertexBuffers() override { return vertexBuffers; }
private:
	unsigned int rendererID = 0;
	std::vector<const VertexBuffer*> vertexBuffers;
};
