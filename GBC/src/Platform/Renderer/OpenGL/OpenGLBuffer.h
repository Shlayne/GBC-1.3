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

		virtual const BufferLayout& GetLayout() const override { return layout; }
		virtual void SetLayout(const BufferLayout& layout) override { this->layout = layout; }
	private:
		RendererID rendererID = 0;
		BufferLayout layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count, const void* data, BufferUsage usage, BufferIndexType type);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(uint32_t count, const void* data) override;

		virtual uint32_t GetCount() const override { return count; }
		virtual BufferIndexType GetType() const override { return type; }
	private:
		RendererID rendererID = 0;
		uint32_t count = 0;
		BufferIndexType type;
	};
}
