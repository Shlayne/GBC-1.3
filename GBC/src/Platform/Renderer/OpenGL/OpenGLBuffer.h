#pragma once

#include "GBC/Rendering/Buffer.h"

namespace gbc
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, const void* data, BufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(uint32_t size, const void* data) override;

		virtual const VertexBufferLayout& GetLayout() const override { return layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { this->layout = layout; }
	private:
		RendererID rendererID = 0;
		VertexBufferLayout layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count, const void* data, BufferUsage usage, IndexBufferElementType type);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(uint32_t count, const void* data) override;

		virtual uint32_t GetCount() const override { return count; }
		virtual IndexBufferElementType GetType() const override { return type; }
	private:
		RendererID rendererID = 0;
		uint32_t count = 0;
		IndexBufferElementType type;
	};
}
