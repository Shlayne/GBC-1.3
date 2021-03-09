#pragma once

#include "Rendering/Buffer.h"

namespace cbc
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(unsigned int size, const void* data, BufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(unsigned int size, const void* data) override;

		virtual const BufferLayout& GetLayout() const override { return layout; }
		virtual void SetLayout(const BufferLayout& layout) override { this->layout = layout; }
	private:
		RendererID rendererID = 0;
		BufferLayout layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int count, const void* data, BufferUsage usage);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(unsigned int count, const void* data) override;

		virtual unsigned int GetCount() const override { return count; }
	private:
		RendererID rendererID = 0;
		unsigned int count = 0;
	};
}
