#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "GBC/Core/Core.h"

namespace gbc
{
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

	enum class VertexBufferElementType : uint8_t
	{
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Float, Float2, Float3, Float4
	};

	struct VertexBufferElement
	{
		VertexBufferElement(VertexBufferElementType type, const std::string& name, bool normalized = false) noexcept;

		VertexBufferElementType type;
		std::string name;
		bool normalized;
		int count;
		int size;
		int offset = 0;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() noexcept = default;
		VertexBufferLayout(std::initializer_list<VertexBufferElement> elements) noexcept;

		inline const std::vector<VertexBufferElement>& GetElements() const noexcept { return elements; }
		inline int GetStride() const noexcept { return stride; }
	private:
		std::vector<VertexBufferElement> elements;
		int stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(uint32_t size, const void* data) = 0;

		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static Ref<VertexBuffer> CreateRef(uint32_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
		static Scope<VertexBuffer> CreateScope(uint32_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	};

	enum class IndexBufferElementType
	{
		UInt32,
		UInt16,
		UInt8,
	};

	static uint32_t GetIndexBufferElementSize(IndexBufferElementType type)
	{
		switch (type)
		{
			case IndexBufferElementType::UInt32: return sizeof(uint32_t);
			case IndexBufferElementType::UInt16: return sizeof(uint16_t);
			case IndexBufferElementType::UInt8:  return sizeof(uint8_t);
		}

		GBC_CORE_ASSERT(false, "Unknown Index Buffer Element Type!");
		return 0;
	}

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(uint32_t count, const void* data) = 0;

		virtual uint32_t GetCount() const = 0;
		virtual IndexBufferElementType GetType() const = 0;

		static Ref<IndexBuffer> CreateRef(uint32_t count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw, IndexBufferElementType type = IndexBufferElementType::UInt32);
		static Scope<IndexBuffer> CreateScope(uint32_t count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw, IndexBufferElementType type = IndexBufferElementType::UInt32);
	};
}
