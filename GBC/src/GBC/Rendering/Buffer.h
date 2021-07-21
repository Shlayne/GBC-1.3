#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace gbc
{
	enum class BufferElementType : uint8_t
	{
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Float, Float2, Float3, Float4
	};

	struct BufferElement
	{
		BufferElement(BufferElementType type, const std::string& name, bool normalized = false) noexcept;

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
		BufferLayout() noexcept = default;
		BufferLayout(std::initializer_list<BufferElement> elements) noexcept;

		inline const std::vector<BufferElement>& GetElements() const noexcept { return elements; }
		inline int GetStride() const noexcept { return stride; }
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

		virtual void SetData(uint32_t size, const void* data) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> CreateRef(uint32_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
		static Scope<VertexBuffer> CreateScope(uint32_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	};

	enum class BufferIndexType
	{
		UInt32 = 4,
		UInt16 = 2,
		UInt8 = 1
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(uint32_t count, const void* data) = 0;

		virtual uint32_t GetCount() const = 0;
		virtual BufferIndexType GetType() const = 0;

		static Ref<IndexBuffer> CreateRef(uint32_t count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw, BufferIndexType type = BufferIndexType::UInt32);
		static Scope<IndexBuffer> CreateScope(uint32_t count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw, BufferIndexType type = BufferIndexType::UInt32);
	};
}
