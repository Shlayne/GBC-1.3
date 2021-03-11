#pragma once

#include "GBC/Rendering/VertexArray.h"

namespace gbc
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override { return vertexBuffers; }
	private:
		RendererID rendererID = 0;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
	};
}
