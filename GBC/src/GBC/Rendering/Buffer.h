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
		int32_t count;
		int32_t size;
		int32_t offset = 0;
	};

	class VertexBufferLayout
	{
	public:
		constexpr VertexBufferLayout() noexcept = default;
		constexpr VertexBufferLayout(std::initializer_list<VertexBufferElement> elements) noexcept
			: elements(elements)
		{
			for (VertexBufferElement& element : this->elements)
			{
				element.offset = stride;
				stride += element.size;
			}
		}

		constexpr const std::vector<VertexBufferElement>& GetElements() const noexcept { return elements; }
		constexpr int32_t GetStride() const noexcept { return stride; }

		constexpr auto begin() noexcept { return elements.begin(); }
		constexpr auto begin() const noexcept { return elements.begin(); }
		constexpr auto end() noexcept { return elements.end(); }
		constexpr auto end() const noexcept { return elements.end(); }
		constexpr auto rbegin() noexcept { return elements.rbegin(); }
		constexpr auto rbegin() const noexcept { return elements.rbegin(); }
		constexpr auto rend() noexcept { return elements.rend(); }
		constexpr auto rend() const noexcept { return elements.rend(); }
		constexpr auto cbegin() const noexcept { return elements.cbegin(); }
		constexpr auto cend() const noexcept { return elements.cend(); }
		constexpr auto crbegin() const noexcept { return elements.crbegin(); }
		constexpr auto crend() const noexcept { return elements.crend(); }
	private:
		std::vector<VertexBufferElement> elements;
		int32_t stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
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

		virtual void SetData(const void* data, uint32_t count) = 0;

		virtual uint32_t GetCount() const = 0;
		virtual IndexBufferElementType GetType() const = 0;

		static Ref<IndexBuffer> Create(uint32_t count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw, IndexBufferElementType type = IndexBufferElementType::UInt32);
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	};
}
