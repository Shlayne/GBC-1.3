#pragma once

#include <string>

namespace gbc
{
	enum class BufferElementType : unsigned char
	{
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Float, Float2, Float3, Float4
	};

	struct BufferElement
	{
		BufferElement(BufferElementType type, const std::string& name, bool normalized = false);

		BufferElementType type;
		std::string name;
		bool normalized;
		int count;
		int size;
		int offset = 0;
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(std::initializer_list<BufferElement> elements);

		inline const std::vector<BufferElement>& GetElements() const { return elements; }
		inline int GetStride() const { return stride; }
	private:
		std::vector<BufferElement> elements;
		int stride = 0;
	};

	enum class BufferUsage
	{
		StreamDraw,
		StreamRead,
		StreamCopy,
		StaticDraw,
		StaticRead,
		StaticCopy,
		DynamicDraw,
		DynamicRead,
		DynamicCopy
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(unsigned int size, const void* data) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> CreateRef(unsigned int size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
		static Scope<VertexBuffer> CreateScope(unsigned int size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(unsigned int count, const void* data) = 0;

		virtual unsigned int GetCount() const = 0;

		static Ref<IndexBuffer> CreateRef(unsigned int count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
		static Scope<IndexBuffer> CreateScope(unsigned int count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	};
}
