#pragma once

namespace gbc
{
	enum class BufferElementType : unsigned char
	{
		Int    = 0x81,
		Int2   = 0x82,
		Int3   = 0x83,
		Int4   = 0x84,
		Float  = 0x01,
		Float2 = 0x02,
		Float3 = 0x03,
		Float4 = 0x04
	};

	struct BufferElement
	{
		BufferElement(BufferElementType type, bool normalized = false)
			: type(type), normalized(normalized) {}

		BufferElementType type;
		bool normalized;
		int size = 0;
		int count = 0;
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
